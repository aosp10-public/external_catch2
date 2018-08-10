/*
 *  Created by Martin on 25/07/2017.
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include "catch_test_registry.h"
#include "catch_test_case_registry_impl.h"
#include "catch_interfaces_registry_hub.h"

namespace Catch {

    auto makeTestInvoker( void(*testAsFunction)() ) noexcept -> ITestInvoker* {
        return new(std::nothrow) TestInvokerAsFunction( testAsFunction );
    }

    NameAndTags::NameAndTags( StringRef name_ , StringRef tags_ ) noexcept : name( name_ ), tags( tags_ ) {}

    AutoReg::AutoReg( ITestInvoker* invoker, SourceLineInfo const& lineInfo, StringRef classOrMethod, NameAndTags const& nameAndTags ) noexcept {
        CATCH_INTERNAL_TRY {
            getMutableRegistryHub()
                    .registerTest(
                        makeTestCase(
                            invoker,
                            extractClassName( classOrMethod ),
                            nameAndTags.name,
                            nameAndTags.tags,
                            lineInfo));
        } CATCH_INTERNAL_CATCH_ALL() {
            // Do not throw when constructing global objects, instead register the exception to be processed later
            getMutableRegistryHub().registerStartupException();
        }
    }
    
    AutoReg::~AutoReg() = default;
}
