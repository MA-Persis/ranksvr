#include "ranker/rank_adapter.h"

#include <pthread.h>

#include "config/ConfigReader.h"
#include "config/JsonConfigLoader.h"

#include "request/request.h"
#include "ranker/ranker.h"
#include "search/search_ranker.h"
#include "mobsearch/mobsearch_ranker.h"

#include <assert.h>

#define RANKTYPE "ranktype"
#define MODULES "modules"

static pthread_mutex_t g_init_mutex = PTHREAD_MUTEX_INITIALIZER;

namespace qihoo { namespace ad { namespace ranking {

using namespace example;
using namespace adinfo;
using namespace std;

extern "C" void CreateRanking(RankType **p) {
    *p = new qihoo::ad::ranking::RankAdapter();
}

extern "C" void DestroyRanking(RankType **p) {
    if (NULL != p) {
        delete *p;
    }
}

RankAdapter::RankAdapter()
    : _ranker(NULL) {
}

RankAdapter::~RankAdapter()
{
  if (_ranker) { 
    delete _ranker;
    _ranker = NULL;
  }
}

bool RankAdapter::Init(const string& conf_file) {
  pthread_mutex_lock(&g_init_mutex);
  const char* real_file = conf_file.c_str();

  ConfigMap cfg_map;
  if(!real_file || !JsonConfigLoader::Load(real_file, cfg_map)) {
      pthread_mutex_unlock(&g_init_mutex);

      LOG(FATAL) << "load config wrong:" << real_file;
      return false;
  }

  string format;

  if (!JsonConfigLoader::Get(cfg_map, RANKTYPE, format)) {
      LOG(FATAL) << "rank type failed";
      pthread_mutex_unlock(&g_init_mutex);
      return false;
  }
  if (!format.compare("search-online")) {
      _ranker = new SearchOnlineRanker;
  } else if (!format.compare("mobsearch-online")) {
      _ranker = new MobSearchOnlineRanker;
  } else {
      LOG(FATAL) << "init adapter failed";
      pthread_mutex_unlock(&g_init_mutex);
      return false;
  }

  if (!JsonConfigLoader::Get(cfg_map, MODULES, format)) {
      LOG(FATAL) << "rank conf failed";
      pthread_mutex_unlock(&g_init_mutex);
      return false;
  }

  if (!_ranker->Build(format)) {
      LOG(FATAL) << "init ranker failed";
      pthread_mutex_unlock(&g_init_mutex);
      return false;
  }

  pthread_mutex_unlock(&g_init_mutex);
  LOG(NOTICE) << "init ranker ok";

  return true;
}

int RankAdapter::GetFeatureKey(example::RankStruct &rank_struct)
{
    if (!_request.search.SetInstance(&rank_struct)
      || 0 != _ranker->GetFeatureKey(rank_struct, _request, _result)) {
        LOG(FATAL) << "GetFeatureKey error";
        return -1;
    }

    return 0;
}

int RankAdapter::DoRank(example::RankStruct &rank_struct)
{
    if (!_request.search.SetInstance(&rank_struct)
        || 0 != _ranker->DoRank(rank_struct, _request, _result)) {
        LOG(FATAL) << "DoRank error";
        return -1;
    }

    return 0;
}

} } }
