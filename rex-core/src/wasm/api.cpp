#if REX_OS_WASM

#include "rex-core/api.h"
#include "rex-core/path.h"
#include "rex-core/str.h"
#include "rex-core/assert.h"

#if REX_HOT_RELOAD == 0
extern "C" Rex_Api* rex_api(Rex_Api* api, bool reload);
#endif

struct Rex_Api_RAII
{
	Rex_Api* api = nullptr;
	rex_api_proc proc = nullptr;

	~Rex_Api_RAII()
	{
		if (api && proc)
			api = proc(api, false);
	}
};

Rex_Api*
load_rex_api()
{
	static Rex_Api_RAII rex;

#if REX_HOT_RELOAD == 1
# error "implement hot reloading"
#else
	if (rex.api == nullptr)
	{
		rex.api = rex_api(nullptr, false);
		rex.proc = rex_api;
	}
#endif
	return rex.api;
}

#endif
