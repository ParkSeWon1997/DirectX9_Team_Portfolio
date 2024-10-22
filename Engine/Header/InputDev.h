#ifndef InputDev_h__
#define InputDev_h__

#include "Engine_Define.h"
#include "Base.h"

namespace Engine {
	enum class eKEY_STATE
	{
		NONE,
		TAP, //������
		HOLD,//������ ������
		AWAY, // �H����
		KEY_END,
	};
	struct tKeyInfo
	{
		eKEY_STATE	eState; //Ű�� ���°�
		bool		bPrevPush;	//���������ӿ� �������� �ȴ�������
	};

	class ENGINE_DLL CInputDev : public CBase
	{
		DECLARE_SINGLETON(CInputDev)

	private:
		explicit CInputDev(void);
		virtual ~CInputDev(void);

	public:
		_byte	Get_DIKeyState(_ubyte byKeyID) { return m_byKeyState[byKeyID]; }
		_byte	Get_DIMouseState(MOUSEKEYSTATE eMouse)
		{
			return m_tMouseState.rgbButtons[eMouse];
		}

		_long	Get_DIMouseMove(MOUSEMOVESTATE eMouseState)
		{
			return *(((_long*)&m_tMouseState) + eMouseState);
		}

		eKEY_STATE GetKeyState(_ubyte byKeyID);
		eKEY_STATE GetMouseState(MOUSEKEYSTATE eMouseState);

		public:
		HRESULT Ready_InputDev(HINSTANCE hInst, HWND hWnd);
		void	Update_InputDev(void);
		void Update_MouseState(void);

	private:
		LPDIRECTINPUT8			m_pInputSDK = nullptr;

	private:
		LPDIRECTINPUTDEVICE8	m_pKeyBoard = nullptr;
		LPDIRECTINPUTDEVICE8	m_pMouse = nullptr;

	private:
		_byte					m_byKeyState[256];		// Ű���忡 �ִ� ��� Ű���� �����ϱ� ���� ����
		DIMOUSESTATE			m_tMouseState;

		vector<eKEY_STATE> m_vecMouseState;
		vector< tKeyInfo>		m_vecKey;
	public:
		virtual void	Free(void);

	};
}
#endif // InputDev_h__
