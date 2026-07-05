#pragma once

namespace irg::i18n {

// The set of languages the terminal UI can be displayed in.
// Adding a new language means: add a case here, then handle it
// in Translator's switch statements (the compiler will warn you
// if you miss one, since we avoid default: cases there on purpose).
enum class Language { English, French };

} // namespace irg::i18n
