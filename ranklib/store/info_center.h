// Copyright (c) 2013 Qihoo 360 Technology Co. Ltd
// Author: Charles Wang

#ifndef _QIHOO_AD_RANKING_INFO_CENTER_H_
#define _QIHOO_AD_RANKING_INFO_CENTER_H_
#include "store/info_impl.h"
#include <algorithm>
#include <butil/logging.h>

namespace qihoo { namespace ad { namespace ranking {

#define GET_STORAGE_VALUE(ic, type, index, var) \
  var = (ic).GetStorageVal<type>(Store_T<type>::index);

#define SET_STORAGE_VALUE(ic, type, index, var) \
  (ic).SetStorageVal<type>(Store_T<type>::index, var);

#define GET_STORAGE_ARRAY(ic, type, index, var, sz) \
  var = (ic).GetStorageArr<type>(Store_A<type>::index, sz);

  class InfoCenter 
  {
    public:
      InfoCenter() {}
      ~InfoCenter() {}

      template <typename Type_t>
      inline Type_t* GetStorageVal(Int_const_t fid) {
        Type_t* ps = storage_impl.GetStoragePtrT<Type_t>();
        if (!ps) {
          return NULL;
        }
        return ps+fid;
      }

      template <typename Type_t>
      inline bool SetStorageVal(Int_const_t fid, const Type_t& value) {
        Type_t* ps = storage_impl.GetStoragePtrT<Type_t>();
        if (!ps) {
          return false;
        }
        ps[fid] = value;
        return true;
      }

      template <typename Type_t>
      inline Type_t* GetStorageArr(Int_const_t fid, Int_t& size) {
        Type_t* ps = storage_impl.GetStoragePtrA<Type_t>(fid);
        if (!ps) {
          size = 0;
          return NULL;
        }
        size = storage_impl.GetStorageSizeA<Type_t>(fid);
        return ps;
      }

      template <typename Type_t>
      inline Type_t* AllocStorageArr(Int_const_t fid, Int_t& size) {
        Type_t* ps = storage_impl.GetStoragePtrA<Type_t>(fid);
        if (!ps) {
          size = 0;
          return NULL;
        }
        size = storage_impl.GetStorageCapA<Type_t>(fid);
        return ps;
      }    

      ////////////////////
      // std::copy INSIDE 
      ////////////////////
      template <typename Type_t>
      inline bool AssignStorageArr(
          Int_const_t fid, const Type_t* begin, const Type_t* end) {
        Type_t* ps = storage_impl.GetStoragePtrA<Type_t>(fid);
        if (!ps) {
          return false;
        }
        if(!storage_impl.SetStorageSizeA<Type_t>(fid, end-begin)) {
          LOG(NOTICE) << "InfoCenter AssignStorageArr failed: " << fid << ":" <<  end-begin;
          return false;
        }
        std::copy(begin, end, ps);
        return true;
      }
      ////////////////////

      template <typename Type_t>
      //inline bool SetStorageSizeArr(Int_ctref_t fid, Int_ctref_t size) {
      inline bool SetStorageSizeArr(Int_const_t fid, Int_ctref_t size) {
        return storage_impl.SetStorageSizeA<Type_t>(fid, size);
      }

      template <typename Type_t> 
      inline Int_const_t GetStorageSizeArr(Int_ctref_t fid) const { 
        return storage_impl.GetStorageSizeA<Type_t>(fid);
      }

#if _DEBUG
    public:
#else
    private:
#endif
      StoreImpl storage_impl; // 静态定义的存储，保证性能
  };

} } }

#endif


