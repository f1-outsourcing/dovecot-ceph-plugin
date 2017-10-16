// -*- mode:C++; tab-width:8; c-basic-offset:2; indent-tabs-mode:t -*-
// vim: ts=8 sw=2 smarttab
/*
 * Copyright (c) 2017 Tallence AG and the authors
 *
 * This is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License version 2.1, as published by the Free Software
 * Foundation.  See file COPYING.
 */

#ifndef SRC_LIBRMB_RADOS_MAIL_OBJECT_H_
#define SRC_LIBRMB_RADOS_MAIL_OBJECT_H_

#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include "rados-metadata.h"
#include "rados-types.h"
#include <rados/librados.hpp>

namespace librmb {

using namespace std;
using namespace librados;

class RadosMailObject {
 public:
  RadosMailObject();
  virtual ~RadosMailObject() {}

  void set_oid(const char* _oid) { this->oid = _oid; }
  void set_oid(const string& _oid) { this->oid = _oid; }
  void set_state(const string& _state) { this->state = _state; }
  void set_version(const string& _version) { this->version = _version; }
  void set_guid(const uint8_t* guid);
  void set_mail_size(const uint64_t& _size) { object_size = _size; }
  void set_mail_buffer(char* _mail_buffer) { this->mail_buffer = _mail_buffer; }
  void set_active_op(bool _active) { this->active_op = _active; }
  void set_rados_save_date(const time_t& _save_date) { this->save_date_rados = _save_date; }

  const string get_oid() { return this->oid; }
  const string get_version() { return this->version; }
  const uint64_t& get_mail_size() { return this->object_size; }

  time_t* get_rados_save_date() { return &this->save_date_rados; }
  uint8_t* get_guid_ref() { return guid; }
  char* get_mail_buffer() { return this->mail_buffer; }
  map<string, ceph::bufferlist>* get_metadata() { return &this->attrset; }
  void set_mail_buffer_content_ptr(const void* start) { mail_buffer_start = start; }
  const void* get_mail_buffer_content_ptr() { return mail_buffer_start; }
  map<AioCompletion*, ObjectWriteOperation*>* get_completion_op_map() { return &completion_op; }
  string get_metadata(rbox_metadata_key key) {
    string str_key(1, static_cast<char>(key));
    return get_metadata(str_key);
  }
  const string get_metadata(string& key) {
    string value;
    if (attrset.find(key) != attrset.end()) {
      value = attrset[key].to_str();
    }
    return value;
  }

  bool has_active_op() { return active_op; }
  string to_string(const string& padding);
  void add_metadata(RadosMetadata& metadata) { attrset[metadata.key] = metadata.bl; }

 private:
  string oid;

  string state;
  string version;

  uint8_t guid[GUID_128_SIZE];
  uint64_t object_size;  // byte
  map<AioCompletion*, ObjectWriteOperation*> completion_op;
  const void* mail_buffer_start;

  bool active_op;
  // used as pointer to a buffer_t (to avoid using dovecot datatypes in library)
  char* mail_buffer;
  time_t save_date_rados;

  map<string, ceph::bufferlist> attrset;

 public:
  static const char X_ATTR_VERSION_VALUE[];
  static const char DATA_BUFFER_NAME[];
};

}  // namespace librmb

#endif  // SRC_LIBRMB_RADOS_MAIL_OBJECT_H_
