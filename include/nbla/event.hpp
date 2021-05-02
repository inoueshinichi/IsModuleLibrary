#pragma once

#include <nbla/common.hpp>
#include <nbla/context.hpp>
#include <nbla/defs.hpp>

#include <memory>

namespace Is
{
    namespace nbla
    {
        using std::shared_ptr;

        class NBLA_API Event
        {
        public:
            Event() {}
            virtual ~Event();

            virtual void wait_event(const Context& ctx,
                                    const int async_flags = AsyncFlag::NONE)
            {}
        };

        // エイリアス
        using EventPtr = shared_ptr<Event>;
    }
}