#include "InputDev.h"

IMPLEMENT_SINGLETON(CInputDev)

Engine::CInputDev::CInputDev(void)
{

}

Engine::CInputDev::~CInputDev(void)
{
	Free();
}

eKEY_STATE Engine::CInputDev::GetKeyState(_ubyte byKeyID)
{
	return m_vecKey[byKeyID].eState;
}

eKEY_STATE Engine::CInputDev::GetMouseState(MOUSEKEYSTATE eMouseState)
{
	return m_vecMouseState[eMouseState];
}

HRESULT Engine::CInputDev::Ready_InputDev(HINSTANCE hInst, HWND hWnd)
{

	// DInput �İ�ü�� �����ϴ� �Լ�
	FAILED_CHECK_RETURN(DirectInput8Create(hInst,
											DIRECTINPUT_VERSION,
											IID_IDirectInput8,
											(void**)&m_pInputSDK,
											NULL), E_FAIL);

	// Ű���� ��ü ����
	FAILED_CHECK_RETURN(m_pInputSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, nullptr), E_FAIL);

	// ������ Ű���� ��ü�� ���� ������ �� ��ü���� �����ϴ� �Լ�
	m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard);

	// ��ġ�� ���� �������� �������ִ� �Լ�, (Ŭ���̾�Ʈ�� ���ִ� ���¿��� Ű �Է��� ������ ������ �����ϴ� �Լ�)
	m_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	// ��ġ�� ���� access ������ �޾ƿ��� �Լ�
	m_pKeyBoard->Acquire();


	// ���콺 ��ü ����
	FAILED_CHECK_RETURN(m_pInputSDK->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr), E_FAIL);

	// ������ ���콺 ��ü�� ���� ������ �� ��ü���� �����ϴ� �Լ�
	m_pMouse->SetDataFormat(&c_dfDIMouse);

	// ��ġ�� ���� �������� �������ִ� �Լ�, Ŭ���̾�Ʈ�� ���ִ� ���¿��� Ű �Է��� ������ ������ �����ϴ� �Լ�
	m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	// ��ġ�� ���� access ������ �޾ƿ��� �Լ�
	m_pMouse->Acquire();


	m_vecKey.reserve(DIK_END);

	for (int i = 0; i < 256; ++i)
	{
		m_vecKey.push_back(tKeyInfo{ eKEY_STATE::NONE,false });
	}

	for (int i = 0; i < DIM_END; ++i)
		m_vecMouseState.push_back(eKEY_STATE::NONE);

	return S_OK;
}

void Engine::CInputDev::Update_InputDev(void)
{
	m_pKeyBoard->GetDeviceState(256, m_byKeyState);
	m_pMouse->GetDeviceState(sizeof(m_tMouseState), &m_tMouseState);

	for (size_t i = 0; i < 256; i++)
	{
		if (m_byKeyState[i] & 0x80)
		{
			if (m_vecKey[i].bPrevPush)
			{
				//�������� �����־�����
				m_vecKey[i].eState = eKEY_STATE::HOLD;
			}
			else
			{
				m_vecKey[i].eState = eKEY_STATE::TAP;
			}
			//���������� �����־����� true
			m_vecKey[i].bPrevPush = true;
		}
		else// Ű�� �ȴ����־�����
		{
			if (m_vecKey[i].bPrevPush)
			{
				// ������ �����־���.
				m_vecKey[i].eState = eKEY_STATE::AWAY;
			}
			else
			{
				m_vecKey[i].eState = eKEY_STATE::NONE;

			}
			m_vecKey[i].bPrevPush = false;
		}

	}
}

void Engine::CInputDev::Update_MouseState(void)
{
	for (int i = 0; i < DIM_END; ++i)
	{
		if (m_tMouseState.rgbButtons[i] & 0x80)
		{
			if (m_vecMouseState[i] == eKEY_STATE::NONE)
				m_vecMouseState[i] = eKEY_STATE::TAP;
			else
				m_vecMouseState[i] = eKEY_STATE::HOLD;
		}
		else
		{
			if (m_vecMouseState[i] == eKEY_STATE::HOLD || m_vecMouseState[i] == eKEY_STATE::TAP)
				m_vecMouseState[i] = eKEY_STATE::AWAY;
			else
				m_vecMouseState[i] = eKEY_STATE::NONE;
		}
	}
}

void Engine::CInputDev::Free(void)
{
	Safe_Release(m_pKeyBoard);
	Safe_Release(m_pMouse);
	Safe_Release(m_pInputSDK);
}

