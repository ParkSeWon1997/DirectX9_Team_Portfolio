#ifndef Engine_Macro_h__
#define Engine_Macro_h__

namespace Engine
{
#define	VTXCNTX		129		
#define VTXCNTZ		129
#define VTXITV		1



#define WINCX 1080
#define WINCY 720

#define INVENCNT 15


#ifndef			MSG_BOX
#define			MSG_BOX(_message)			MessageBox(NULL, TEXT(_message), L"System Message", MB_OK)
#endif

#define			DDDD						}


#ifdef	ENGINE_EXPORTS
#define ENGINE_DLL		_declspec(dllexport)
#else
#define ENGINE_DLL		_declspec(dllimport)
#endif

#define NULL_CHECK(_ptr) \
    { if (_ptr == nullptr) { assert(false && "NULL_CHECK failed"); return; } }

#define NULL_CHECK_RETURN(_ptr, _return) \
    { if (_ptr == nullptr) { assert(false && "NULL_CHECK_RETURN failed"); return _return; } }

#define NULL_CHECK_MSG(_ptr, _message) \
    { if (_ptr == nullptr) { MessageBox(NULL, _message, L"System Message", MB_OK); assert(false && "NULL_CHECK_MSG failed"); } }

#define NULL_CHECK_RETURN_MSG(_ptr, _return, _message) \
    { if (_ptr == nullptr) { MessageBox(NULL, _message, L"System Message", MB_OK); assert(false && "NULL_CHECK_RETURN_MSG failed"); return _return; } }

#define FAILED_CHECK(_hr) \
    { if (FAILED(_hr)) { MessageBoxW(NULL, L"Failed", L"System Error", MB_OK); assert(false && "FAILED_CHECK failed"); return E_FAIL; } }

#define FAILED_CHECK_RETURN(_hr, _return) \
    { if (FAILED(_hr)) { MessageBoxW(NULL, L"Failed", L"System Error", MB_OK); assert(false && "FAILED_CHECK_RETURN failed"); return _return; } }

#define FAILED_CHECK_MSG(_hr, _message) \
    { if (FAILED(_hr)) { MessageBoxW(NULL, _message, L"System Message", MB_OK); assert(false && "FAILED_CHECK_MSG failed"); return E_FAIL; } }

#define FAILED_CHECK_RETURN_MSG(_hr, _return, _message) \
    { if (FAILED(_hr)) { MessageBoxW(NULL, _message, L"System Message", MB_OK); assert(false && "FAILED_CHECK_RETURN_MSG failed"); return _return; } }

#define NO_COPY(CLASSNAME)								\
		private:										\
		CLASSNAME(const CLASSNAME&);					\
		CLASSNAME& operator = (const CLASSNAME&);		

#define DECLARE_SINGLETON(CLASSNAME)					\
		NO_COPY(CLASSNAME)								\
		private:										\
		static CLASSNAME*	m_pInstance;				\
		public:											\
		static CLASSNAME*	GetInstance( void );		\
		static void DestroyInstance( void );			

#define IMPLEMENT_SINGLETON(CLASSNAME)					\
		CLASSNAME*	CLASSNAME::m_pInstance = NULL;		\
		CLASSNAME*	CLASSNAME::GetInstance( void )	{	\
			if(NULL == m_pInstance) {					\
				m_pInstance = new CLASSNAME;			\
			}											\
			return m_pInstance;							\
		}												\
		void CLASSNAME::DestroyInstance( void ) {		\
			if(NULL != m_pInstance)	{					\
				delete m_pInstance;						\
				m_pInstance = NULL;						\
			}											\
		}
}


#define KEY_CHECK( key , state) 	Engine::GetKeyState(key) == state
#define KEY_HOLD(key) KEY_CHECK(key,eKEY_STATE::HOLD)
#define KEY_TAP(key) KEY_CHECK(key, eKEY_STATE::TAP)
#define KEY_AWAY(key) KEY_CHECK(key,eKEY_STATE::AWAY )
#define KEY_NONE(key) KEY_CHECK(key,eKEY_STATE::NONE )


#endif // Engine_Macro_h__
