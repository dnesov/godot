/**************************************************************************/
/*  gdscript_warning.h                                                    */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#ifndef GDSCRIPT_WARNING_H
#define GDSCRIPT_WARNING_H

#ifdef DEBUG_ENABLED

#include "core/object/object.h"
#include "core/string/ustring.h"
#include "core/templates/vector.h"

class GDScriptWarning {
public:
	enum WarnLevel {
		IGNORE,
		WARN,
		ERROR
	};

	enum Code {
		UNASSIGNED_VARIABLE, // Variable used but never assigned.
		UNASSIGNED_VARIABLE_OP_ASSIGN, // Variable never assigned but used in an assignment operation (+=, *=, etc).
		UNUSED_VARIABLE, // Local variable is declared but never used.
		UNUSED_LOCAL_CONSTANT, // Local constant is declared but never used.
		SHADOWED_VARIABLE, // Variable name shadowed by other variable in same class.
		SHADOWED_VARIABLE_BASE_CLASS, // Variable name shadowed by other variable in some base class.
		UNUSED_PRIVATE_CLASS_VARIABLE, // Class variable is declared private ("_" prefix) but never used in the file.
		UNUSED_PARAMETER, // Function parameter is never used.
		UNREACHABLE_CODE, // Code after a return statement.
		UNREACHABLE_PATTERN, // Pattern in a match statement after a catch all pattern (wildcard or bind).
		STANDALONE_EXPRESSION, // Expression not assigned to a variable.
		NARROWING_CONVERSION, // Float value into an integer slot, precision is lost.
		INCOMPATIBLE_TERNARY, // Possible values of a ternary if are not mutually compatible.
		UNUSED_SIGNAL, // Signal is defined but never emitted.
		RETURN_VALUE_DISCARDED, // Function call returns something but the value isn't used.
		PROPERTY_USED_AS_FUNCTION, // Function not found, but there's a property with the same name.
		CONSTANT_USED_AS_FUNCTION, // Function not found, but there's a constant with the same name.
		FUNCTION_USED_AS_PROPERTY, // Property not found, but there's a function with the same name.
		INTEGER_DIVISION, // Integer divide by integer, decimal part is discarded.
		UNSAFE_PROPERTY_ACCESS, // Property not found in the detected type (but can be in subtypes).
		UNSAFE_METHOD_ACCESS, // Function not found in the detected type (but can be in subtypes).
		UNSAFE_CAST, // Cast used in an unknown type.
		UNSAFE_CALL_ARGUMENT, // Function call argument is of a supertype of the require argument.
		UNSAFE_VOID_RETURN, // Function returns void but returned a call to a function that can't be type checked.
		DEPRECATED_KEYWORD, // The keyword is deprecated and should be replaced.
		STANDALONE_TERNARY, // Return value of ternary expression is discarded.
		ASSERT_ALWAYS_TRUE, // Expression for assert argument is always true.
		ASSERT_ALWAYS_FALSE, // Expression for assert argument is always false.
		REDUNDANT_AWAIT, // await is used but expression is synchronous (not a signal nor a coroutine).
		EMPTY_FILE, // A script file is empty.
		SHADOWED_GLOBAL_IDENTIFIER, // A global class or function has the same name as variable.
		INT_AS_ENUM_WITHOUT_CAST, // An integer value was used as an enum value without casting.
		INT_AS_ENUM_WITHOUT_MATCH, // An integer value was used as an enum value without matching enum member.
		STATIC_CALLED_ON_INSTANCE, // A static method was called on an instance of a class instead of on the class itself.
		CONFUSABLE_IDENTIFIER, // The identifier contains misleading characters that can be confused. E.g. "usеr" (has Cyrillic "е" instead of Latin "e").
		RENAMED_IN_GD4_HINT, // A variable or function that could not be found has been renamed in Godot 4
		INFERENCE_ON_VARIANT, // The declaration uses type inference but the value is typed as Variant.
		NATIVE_METHOD_OVERRIDE, // The script method overrides a native one, this may not work as intended.
		GET_NODE_DEFAULT_WITHOUT_ONREADY, // A class variable uses `get_node()` (or the `$` notation) as its default value, but does not use the @onready annotation.
		ONREADY_WITH_EXPORT, // The `@onready` annotation will set the value after `@export` which is likely not intended.
		REDUNDANT_STATIC_UNLOAD, // The `@static_unload` annotation is used but the class does not have static data.
		WARNING_MAX,
	};

	constexpr static WarnLevel default_warning_levels[] = {
		WARN, // UNASSIGNED_VARIABLE
		WARN, // UNASSIGNED_VARIABLE_OP_ASSIGN
		WARN, // UNUSED_VARIABLE
		WARN, // UNUSED_LOCAL_CONSTANT
		WARN, // SHADOWED_VARIABLE
		WARN, // SHADOWED_VARIABLE_BASE_CLASS
		WARN, // UNUSED_PRIVATE_CLASS_VARIABLE
		WARN, // UNUSED_PARAMETER
		WARN, // UNREACHABLE_CODE
		WARN, // UNREACHABLE_PATTERN
		WARN, // STANDALONE_EXPRESSION
		WARN, // NARROWING_CONVERSION
		WARN, // INCOMPATIBLE_TERNARY
		WARN, // UNUSED_SIGNAL
		IGNORE, // RETURN_VALUE_DISCARDED // Too spammy by default on common cases (connect, Tween, etc.).
		WARN, // PROPERTY_USED_AS_FUNCTION
		WARN, // CONSTANT_USED_AS_FUNCTION
		WARN, // FUNCTION_USED_AS_PROPERTY
		WARN, // INTEGER_DIVISION
		IGNORE, // UNSAFE_PROPERTY_ACCESS // Too common in untyped scenarios.
		IGNORE, // UNSAFE_METHOD_ACCESS // Too common in untyped scenarios.
		IGNORE, // UNSAFE_CAST // Too common in untyped scenarios.
		IGNORE, // UNSAFE_CALL_ARGUMENT // Too common in untyped scenarios.
		WARN, // UNSAFE_VOID_RETURN
		WARN, // DEPRECATED_KEYWORD
		WARN, // STANDALONE_TERNARY
		WARN, // ASSERT_ALWAYS_TRUE
		WARN, // ASSERT_ALWAYS_FALSE
		WARN, // REDUNDANT_AWAIT
		WARN, // EMPTY_FILE
		WARN, // SHADOWED_GLOBAL_IDENTIFIER
		WARN, // INT_AS_ENUM_WITHOUT_CAST
		WARN, // INT_AS_ENUM_WITHOUT_MATCH
		WARN, // STATIC_CALLED_ON_INSTANCE
		WARN, // CONFUSABLE_IDENTIFIER
		WARN, // RENAMED_IN_GD4_HINT
		ERROR, // INFERENCE_ON_VARIANT // Most likely done by accident, usually inference is trying for a particular type.
		ERROR, // NATIVE_METHOD_OVERRIDE // May not work as expected.
		ERROR, // GET_NODE_DEFAULT_WITHOUT_ONREADY // May not work as expected.
		ERROR, // ONREADY_WITH_EXPORT // May not work as expected.
		WARN, // REDUNDANT_STATIC_UNLOAD
	};

	static_assert((sizeof(default_warning_levels) / sizeof(default_warning_levels[0])) == WARNING_MAX, "Amount of default levels does not match the amount of warnings.");

	Code code = WARNING_MAX;
	int start_line = -1, end_line = -1;
	int leftmost_column = -1, rightmost_column = -1;
	Vector<String> symbols;

	String get_name() const;
	String get_message() const;
	static int get_default_value(Code p_code);
	static PropertyInfo get_property_info(Code p_code);
	static String get_name_from_code(Code p_code);
	static String get_settings_path_from_code(Code p_code);
	static Code get_code_from_name(const String &p_name);
};

#endif // DEBUG_ENABLED

#endif // GDSCRIPT_WARNING_H
