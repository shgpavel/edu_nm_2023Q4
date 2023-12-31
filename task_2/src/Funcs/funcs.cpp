/*
Copyright 2023 Pavel Shago

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include <iostream>

double square_root_pd(double num, double epsilon) {

    double guess = num;
    double prevguess = 0.0;

    while (std::abs(guess - prevguess) > epsilon) {
        prevguess = guess;
        guess = 0.5 * (guess + num / guess);
    }

    return guess;
}
