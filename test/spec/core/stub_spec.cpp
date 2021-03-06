#include <bandit_with_gmock/bandit_with_gmock.hpp>
#include <webmock/core/stub.hpp>

namespace webmock { namespace core {
go_bandit([]{
    using namespace bandit;
    
    describe("webmock::core::stub", []{
        describe("#get_response(request)", [&]{
            it("should return a response specified by this sequences, and if came to an end of the last sequence, then keep returning that response", [&]{
                response res1{"200","test1"};
                response res2{"200","test2"};
                stub target{
                    {[](auto &&){ return true; }},
                    {
                        {2, [&res1](auto &&){ return res1; }},
                        {1, [&res2](auto &&){ return res2; }},
                    }
                };
                AssertThat(target.get_response({}), Equals(res1));
                AssertThat(target.get_response({}), Equals(res1));
                AssertThat(target.get_response({}), Equals(res2));
                AssertThat(target.get_response({}), Equals(res2));
                AssertThat(target.get_response({}), Equals(res2));
            });
            
            describe("when this sequences were empty", [&]{
                it("should be an empty response", [&]{
                    stub target{
                        {[](auto &&){ return true; }},
                        {}
                    };
                    AssertThat(target.get_response({}), Equals(response{}));
                });
            });
        });
    });
});
}}
