#pragma once
namespace ZR
{
	class CObject
	{
	public:
		CObject();
		~CObject();
		void Init(int &argc, char **argv);
		void Show();
		void Idle();
	private:
		static void display(void);
		static void reshap(int w, int h);
		static void OnMouseClick(int button, int state, int x, int y);
		static void OnMouseMove(int x, int y);

	};
}


