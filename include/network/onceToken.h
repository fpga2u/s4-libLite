﻿/*
 * Copyright (c) 2016 The ZLToolKit project authors. All Rights Reserved.
 *
 * This file is part of ZLToolKit(https://github.com/xiongziliang/ZLToolKit).
 *
 * Use of this source code is governed by MIT license that can be found in the
 * LICENSE file in the root of the source tree. All contributing project authors
 * may be found in the AUTHORS file in the root of the source tree.
 */

#ifndef UTIL_ONCETOKEN_H_
#define UTIL_ONCETOKEN_H_

#include <functional>
#include <type_traits>

namespace S4 {

class onceToken {
public:
    typedef std::function<void(void)> task;

    template<typename FUNC>
    onceToken(const FUNC &onConstructed, std::function<void(void)> onDestructed = nullptr) {
        onConstructed();
        _onDestructed = std::move(onDestructed);
    }

    onceToken(nullptr_t, std::function<void(void)> onDestructed = nullptr) {
        _onDestructed = std::move(onDestructed);
    }

    ~onceToken() {
        if (_onDestructed) {
            _onDestructed();
        }
    }

private:
    onceToken() = delete;
    onceToken(const onceToken &) = delete;
    onceToken(onceToken &&) = delete;
    onceToken &operator=(const onceToken &) = delete;
    onceToken &operator=(onceToken &&) = delete;

private:
    task _onDestructed;
};

} /* namespace S4 */
#endif /* UTIL_ONCETOKEN_H_ */
