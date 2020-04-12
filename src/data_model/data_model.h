/*!
 * @file
 * @brief
 */

#ifndef data_model_h
#define data_model_h

#include "tiny_ram_key_value_store.h"
#include "data_model_key_value_pairs.h"

#define _storage_type_name data_model_storage_t
generate_storage_type_for_ram_key_value_pairs(data_model_key_value_pairs);

typedef struct {
  data_model_storage_t storage;
  tiny_ram_key_value_store_t key_value_store;
} data_model_t;

void data_model_init(data_model_t* self);

i_tiny_key_value_store_t* data_model_key_value_store(data_model_t* self);

#endif
