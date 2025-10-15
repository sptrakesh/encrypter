//
// Created by Rakesh on 15/10/2025.
//

#pragma once

#include "rust/cxx.h"
#include "encrypter/src/lib.rs.h"

void init( const Configuration& conf );
rust::String encrypt( rust::Str data );
rust::String decrypt( rust::Str data );
