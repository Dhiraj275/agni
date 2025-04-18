
#define AGWND_EXCEPT( hr ) Window::Exception( __LINE__,__FILE__,hr )
#define AGWND_LAST_EXCEPT() Window::Exception( __LINE__,__FILE__,GetLastError() )

// This would be in a header file somewhere
#if defined(IS_DEBUG) && IS_DEBUG
constexpr bool is_debug = true;
#else
constexpr bool is_debug = false;
#endif