/**
 * @file ui_def.h
 * @brief Status codes for application UI plugin operations.
 */

#ifndef UI_DEF_H
#define UI_DEF_H

namespace Ui
{
    /**
     * @brief Defines UI plugin operation status; the success value is zero.
     */
    enum class ErrorCode
    {
        NoError ///< The plugin operation reports success.
    };
}

#endif // UI_DEF_H
