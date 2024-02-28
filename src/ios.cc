#ifdef ARDUINO_ARCH_SAM
#include "bits/ios_base.h"
namespace std
{
	// 71
	_Atomic_word ios_base::Init::_S_refcount;

	bool ios_base::Init::_S_synced_with_stdio = true;

	ios_base::ios_base() throw()
		: _M_precision(), _M_width(), _M_flags(), _M_exception(),
		  _M_streambuf_state(), _M_callbacks(0), _M_word_zero(),
		  _M_word_size(_S_local_word_size), _M_word(_M_local_word), _M_ios_locale()
	{
		// Do nothing: basic_ios::init() does it.
		// NB: _M_callbacks and _M_word must be zero for non-initialized
		// ios_base to go through ~ios_base gracefully.
	}

	// 27.4.2.7  ios_base constructors/destructors
	ios_base::~ios_base()
	{
		_M_call_callbacks(erase_event);
		_M_dispose_callbacks();
		if (_M_word != _M_local_word)
		{
			delete[] _M_word;
			_M_word = 0;
		}
	}
	// 97
}
#endif