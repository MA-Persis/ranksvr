#ifndef QIHOO_AD_RANKING_RANK_ADAPTER_H_
#define QIHOO_AD_RANKING_RANK_ADAPTER_H_

#include <string>
#include <vector>

#include <butil/logging.h>

#include "base/rank.h"
#include "request/request.h"

namespace qihoo { namespace ad { namespace ranking{

class RankerBase;

class RankAdapter: public Rank
{
public:
    // for instance
    RankAdapter();
    virtual ~RankAdapter();

    virtual bool Init(const std::string& conf_file);

    // /*
    //  * 抽取排序特征，返回所有广告特征的key，引擎得到key之后查询特征服务器得到feature_weight。
    //  * 注意这里的版本号：
    //  * 更新模型的时候会先向KV服务器同步数据，同步完成之后将最新的版本号写到zookeeper，在引
    //  * 擎代码当中可以获取到此版本信息，即server_version，排序代码当中无法获取此信息。
    //  * 排序端读取的DNN模型文件当中也存有版本信息，排序维护的模型文件是双buffer的，因此有两
    //  * 个版本，即model_version_new和model_version_last，这个接口会将两个版本都传递出去，最终
    //  * 在引擎代码当中核对server_version，以便确认使用哪一个版本的数据
    //  *
    //  * @param rank_struct [in/out]广告基础信息，排序抽取完特征之后会放到这个结构体对应的位置当中
    //  * @param model_version_prefer [out]模型的最新版本
    //  * @param model_version_backup [out]模型的上一个版本
    //  * @return 成功为0，失败为非0
    //  */
    virtual int GetFeatureKey(example::RankStruct &rank_struct);

    // /*
    //  * 引擎传递获取到的特征weight之后，调用这个函数完成排序。
    //  *
    //  * @param rank_struct [in]广告基础信息
    //  * @param server_model_version [in]引擎获取到的模型的版本号
    //  * @return 成功为0，失败为非0
    //  */
    virtual int DoRank(example::RankStruct &rank_struct);

    // // online rank
    // virtual bool OnlineRank(example::RankStruct &rank_struct);

private:
    RankerBase* _ranker;
    iRequest _request;
    iResult _result;
};

} } }

#endif
