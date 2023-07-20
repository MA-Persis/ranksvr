// Copyright (c) 2013 Qihoo 360 Technology Co. Ltd
// Author: Charles Wang

#ifndef _QIHOO_AD_RANKING_INFO_DEFS_H_
#define _QIHOO_AD_RANKING_INFO_DEFS_H_
#include <string.h>
#include <vector>
#include <iostream>

namespace qihoo { namespace ad { namespace ranking {

//////////Simple Type Store//////////
#define BEGIN_STORAGE_VAL(Type_t, Group_i) \
  template <> \
  class Store_T<Type_t> \
  { \
    public: \
      Store_T() { \
        InitStorage(); \
      } \
      static Int_const_t arr_grp = Group_i; \
      std::vector<Type_t> arr_imp; \
      inline Type_t* GetStoragePtr() { \
        return arr_imp.data(); \
        /*return arr_imp;*/ \
      } \
      inline void ClearStorage() { \
        arr_imp.clear(); \
        /*memset(arr_imp,0,sizeof(arr_imp));*/ \
      } 

#define ADD_STORAGE_VAL(Name_s, Index_i) \
    public: \
      static Int_const_t Name_s = Index_i; 

#define FINISH_STORAGE_VAL(Size_i) \
    public: \
      inline void InitStorage() { \
        arr_imp.resize(Size_i); \
      } \
      static Int_const_t arr_max = Size_i; \
      /*Type_t arr_imp[Size_i];*/ \
  };

//////////Simple Temp. Spec.//////////
#define DEFINE_STORAGE_VAL(Type_t) \
    Store_T<Type_t> Type_t##_impl_t;

#define IMPLEMENT_STORAGE_VAL(Type_t) \
  template <> \
  inline const Store_T<Type_t>* StoreImpl::GetStorageT<Type_t>() const { \
    return &Type_t##_impl_t; \
  } \
  template <> \
  inline Type_t* StoreImpl::GetStoragePtrT<Type_t>() { \
    return Type_t##_impl_t.GetStoragePtr(); \
  } \
  template <> \
  inline void StoreImpl::ClearStorageT<Type_t>() { \
    return Type_t##_impl_t.ClearStorage(); \
  } 

//////////Array Type Store//////////
#define BEGIN_STORAGE_ARR(Type_t, Group_i) \
  template <> \
  class Store_A<Type_t> \
  { \
    public: \
      static Int_const_t arr_grp = Group_i; \
      std::vector<Type_t> arr_imp; \
      Store_A() { \
        InitStorage(); \
      } \
      inline Type_t* GetStoragePtr(Int_ctref_t idx) { \
        return arr_imp.data() + arr_head[idx]; \
      } \
      inline bool SetStorageSize(Int_ctref_t idx, Int_ctref_t size) { \
        if (size > (arr_tail[idx]-arr_head[idx])) { \
          return false; \
        } \
        arr_size[idx] = size; \
        return true; \
      } \
      inline Int_const_t GetStorageSize(Int_ctref_t idx) const { \
        return arr_size[idx]; \
      } \
      inline Int_const_t GetStorageCap(Int_ctref_t idx) const { \
        return arr_tail[idx] - arr_head[idx]; \
      } \
      inline void ClearStorage() { \
        arr_imp.clear(); \
      } 

#define ADD_STORAGE_ARR(Name_s, Index_i) \
      static Int_const_t Name_s = Index_i; 

#define MIDDLE_STORAGE_ARR \
      inline void InitStorage() {

#define LOAD_STORAGE_ARR(Name_i, Len_i) \
        arr_size[Name_i] = 0; \
        if (Name_i > 0) { \
          arr_head[Name_i] = arr_tail[Name_i-1]; \
          arr_tail[Name_i] = arr_head[Name_i] + Len_i; \
        } else if (Name_i == 0) { \
          arr_head[0] = 0; \
          arr_tail[0] = Len_i; \
        } else { \
          std::cerr << "error:" << __FILE__ << ":" << __LINE__ << std::endl; \
        } 

#define FINISH_STORAGE_ARR(Size_i) \
        arr_imp.resize(arr_tail[arr_max-1]); \
      } \
    public: \
      static Int_const_t arr_max = Size_i; \
      Int_t arr_head[Size_i]; \
      Int_t arr_tail[Size_i]; \
      Int_t arr_size[Size_i]; \
  };

//////////Array Temp. Spec.//////////
#define DEFINE_STORAGE_ARR(Type_t) \
    Store_A<Type_t> Type_t##_impl_v;

#define IMPLEMENT_STORAGE_ARR(Type_t) \
  template <> \
  inline const Store_A<Type_t>* StoreImpl::GetStorageA<Type_t>() const { \
    return &Type_t##_impl_v; \
  } \
  template <> \
  inline Type_t* StoreImpl::GetStoragePtrA<Type_t>(Int_ctref_t idx) { \
    return Type_t##_impl_v.GetStoragePtr(idx); \
  } \
  template <> \
  inline bool StoreImpl::SetStorageSizeA<Type_t>(Int_ctref_t idx, Int_ctref_t size) { \
    return Type_t##_impl_v.SetStorageSize(idx, size); \
  } \
  template <> \
  inline Int_const_t StoreImpl::GetStorageSizeA<Type_t>(Int_ctref_t idx) const { \
    return Type_t##_impl_v.GetStorageSize(idx); \
  } \
  template <> \
  inline Int_const_t StoreImpl::GetStorageCapA<Type_t>(Int_ctref_t idx) const { \
    return Type_t##_impl_v.GetStorageCap(idx); \
  } \
  template <> \
  inline void StoreImpl::ClearStorageA<Type_t>() { \
    return Type_t##_impl_v.ClearStorage(); \
  } 

  //////////////////////////
  template <typename Type_t> 
  class Store_T
  {
    // nonempty class
    typedef Type_t const Type_const_t; 
  };

  template <typename Type_t> 
  class Store_A
  {
    // nonempty class
    typedef Type_t const Type_const_t; 
  };

} } }

#endif

