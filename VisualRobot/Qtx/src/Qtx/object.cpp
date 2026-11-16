/**
 * @file object.cpp
 * @brief Private implementation allocation and binding to the owning object.
 */

#include "object.h"
#include "object_p.h"

Qtx::Object::Object()
:Object{new(std::nothrow) ObjectPrivate}
{

}

/**
 * @brief Releases owned private data where its type is complete.
 */
Qtx::Object::~Object() = default;

Qtx::Object::Object(ObjectPrivate *dd)
    :d_ptr{ dd }
{
    if (dd)
    {
        dd->q_ptr = this;
    }
}
