
#ifndef __AVEJ_APP_H
#define __AVEJ_APP_H

/*!
 * \file
 * \ingroup AVEJ library
 * \brief AVEJ library의 graphics interface
 * \author SMgal smgal@hitel.net
 * \date 2006-12-31
*/

#define THIS_CLASS_IS_A_SINGLETON(class_name)      \
	friend class ::avej_lite::singleton<class_name>;    \
	friend class ::avej_lite::auto_deletor<class_name>; \
	class_name();                                  \
	class_name(const class_name&);                 \
	~class_name();                                 \
	class_name& operator=(const class_name&)

#define THIS_STRUCT_IS_A_SINGLETON(struct_name)    \
	private:                                       \
	THIS_CLASS_IS_A_SINGLETON(struct_name);        \
	public:                                        \


namespace avej_lite
{
	template <typename TBaseClass>
	class auto_deletor
	{
	public:
		auto_deletor(TBaseClass* object = 0)
			: m_object(object)
		{
		}
		~auto_deletor(void)
		{
			delete m_object;
		}
		void set(TBaseClass* object)
		{
			m_object = object;
		}

	private:
		TBaseClass* m_object;
	};

	//! 싱글톤 객체를 만들어 주기 위한 템플릿 (single thread 전용)
	/*!
	 * \ingroup gfx2d
	 */
	template <typename TBaseClass, int kind = 0>
	class singleton
	{
	public:
		static TBaseClass& get(int param = 0)
		{
			if (m_instance == 0)
			{
				m_instance = new TBaseClass;
				m_deletor.set(m_instance);
			}

			return *m_instance;
		}
		// 명시적인 해제 순서가 필요할 때 사용한다.
		static void release(int param = 0)
		{
			if (m_instance)
			{
				delete m_instance;
				m_instance = 0;
				m_deletor.set(m_instance);
			}
		}
	private:
		singleton(void);

		static TBaseClass* m_instance;
		static auto_deletor<TBaseClass> m_deletor;
	};

	template <class TBaseClass, int kind> TBaseClass* singleton<TBaseClass, kind>::m_instance = 0;
	template <class TBaseClass, int kind> auto_deletor<TBaseClass> singleton<TBaseClass, kind>::m_deletor;
} // namespace avej_lite

namespace avej_lite
{
	////////////////////////////////////////////////////////////////////////////
	// class CInputDevice

	//! 입력 디바이스의 키 정의
	enum TInputKey
	{
		INPUT_KEY_BEGIN = 0,  //!< 키 값으로 배정된 값의 시작 값
		INPUT_KEY_UP    = 0,  //!< 위쪽 방향 키
		INPUT_KEY_DOWN,       //!< 아래쪽 방향 키
		INPUT_KEY_LEFT,       //!< 왼쪽 방향 키
		INPUT_KEY_RIGHT,      //!< 오른쪽 방향 키
		INPUT_KEY_A,          //!< 일반 누름 버튼 A
		INPUT_KEY_B,          //!< 일반 누름 버튼 B
		INPUT_KEY_C,          //!< 일반 누름 버튼 C
		INPUT_KEY_D,          //!< 일반 누름 버튼 D
		INPUT_KEY_L1,         //!< 왼쪽 트리거 버튼 1
		INPUT_KEY_R1,         //!< 오른쪽 트리거 버튼 1
		INPUT_KEY_SYS1,       //!< 시스템 버튼 1 (start 등)
		INPUT_KEY_SYS2,       //!< 시스템 버튼 2 (select 등)
		INPUT_KEY_SYS3,       //!< 시스템 버튼 3 (vol up 등)
		INPUT_KEY_SYS4,       //!< 시스템 버튼 4 (vol down 등)
		INPUT_KEY_END,        //!< 키 값으로 배정된 값의 마지막 값 (유효하지 않은 값)
		INPUT_KEY_MAX = INPUT_KEY_END - INPUT_KEY_BEGIN
	};

	struct TMultiTouchInfo
	{
		int x, y;

		TMultiTouchInfo()
			: x(-1), y(-1)
		{
		}
	};

	enum { MAX_MULTI_TOUCH = 10 };

	typedef TMultiTouchInfo TMultiTouchInfoArray[MAX_MULTI_TOUCH];
	/*!
	 * \ingroup gfx2d
	 */
	class CInputDevice
	{
		friend class singleton<CInputDevice>;
		friend class auto_deletor<CInputDevice>;

	public:
		//! 입력 디바이스 상태를 현재 상태로 갱신함
		void UpdateInputState();
		//! 지정한 키가 현재 눌러져 있는 상태인지 판별
		bool IsKeyHeldDown(TInputKey key) const;
		//! 지정한 키를 누른 적이 있는 상태인지 판별
		bool WasKeyPressed(TInputKey key) const;
		//!
		bool GetMultiTouchInfo(TMultiTouchInfoArray& multi_touch_info_array) const;

	private:
		CInputDevice();
		~CInputDevice();
	};

} // namespace avej_lite


namespace avej_lite
{

//! Application의 call-back 함수 정의를 위한 structure. Event-driven 방식을 가능하게 한다.
/*!
 * \ingroup AVEJ library application
*/
struct AppCallback
{
	//! Application이 생성될 때 불린다.
	bool (*OnCreate)(void);
	//! Application이 종료될 때 불린다.
	bool (*OnDestory)(void);
	//! 게임의 메인 루프 진행을 위해 계속 불린다.
	bool (*OnProcess)(void);
};

//! Application를 위한 interface
/*!
 * \ingroup AVEJ library application
*/
class IAvejApp
{
public:
	virtual          ~IAvejApp(void) {};
	virtual bool      Process(void) = 0;
	static  void      ProcessMessages(void);
	static  IAvejApp* GetInstance(const AppCallback& callBack);
    static  int       Release(void);
};

class IAvejAppTarget: IAvejApp
{
public:
	IAvejAppTarget(void);
	~IAvejAppTarget(void);

	void  Create(void);
	void  Destory(void);
	bool  Process(void);
};

}

#endif // #ifndef __AVEJ_APP_H
