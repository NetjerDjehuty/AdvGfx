// GLViewer.h

#pragma once

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Collections::Generic;
using namespace System::Threading;
using namespace System::Drawing;

#define Action Tuple<Delegate^,array<Object^>^>

namespace GLViewer {

	// This UserControl is used to render OpenGL graphics.
	// It will update automagically using its own rendering thread, separate from the UI thread.
	public ref class GLViewer : public UserControl
	{
	public:

		// ctor
		GLViewer();

		// OpenGL related events

		// Occurs after the OpenGL context has initialized
		event EventHandler^ GLInit;
		// Occurs when the scene needs to redraw itself
		event EventHandler^ GLDraw;
		// Occurs when a key is down
		event KeyEventHandler^ GLKeyDown;
		// Occurs when a mousekey is pressed
		event MouseEventHandler^ GLMouseDown;
		// Occurs when the mousewheel is scrolled
		event MouseEventHandler^ GLMouseScroll;
		// Occurs when the mouse is dragged
		event MouseEventHandler^ GLMouseMove;


		// methods

		virtual void Refresh() override;
		void SwapBackBuffers();

	private:

		// Initializes the OpenGL context
		void InitContext(HWND WindowHandle);
		// Releases the OpenGL context
		void ReleaseContext(HWND WindowHandle);

		// Creates the rendering thread
		void CreateRThread();
		// Starts the rendering thread
		void StartRThread();
		// Stops the rendering thread
		void StopRThread();
		// The delegate for the rendering thread
		void Execute(Object^ WindowHandle);

		// Enqueue an action to be performed on the rendering thread
		void QueueActions(Delegate^ AsyncAction, ... array<Object^>^ Args);
		// Runs all enqueued actions
		void RunActions();

		bool _initialized;
		HDC _hdc;
		HGLRC _hrc;
		Thread^ _renderThread;
		ManualResetEvent^ _terminated;
		String^ _errMsg;

		// TODO: rework this (maybe use a wait free implementation?
		Queue<Action^>^ ActionQueue;
		Object^ _queueLock;


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>d
		void InitializeComponent(void)
		{
			this->SuspendLayout();
			// 
			// GLViewer
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->DoubleBuffered = true;
			this->Name = L"GLViewer";
			this->Size = System::Drawing::Size(146, 146);
			this->ResumeLayout(false);

		}
#pragma endregion

	protected:
		// dtor
		~GLViewer();

		// 

		virtual void OnPaint(PaintEventArgs^ e) override;
		virtual void OnPaintBackground(PaintEventArgs^ e) override;
		virtual void OnResize(EventArgs^ e) override;
		virtual void OnLoad(EventArgs^ e) override;
		virtual void OnKeyDown(KeyEventArgs^ e) override;
		virtual void OnMouseDown(MouseEventArgs^ e) override;
		virtual void OnMouseWheel(MouseEventArgs^ e) override;
		virtual void OnMouseMove(MouseEventArgs^ e) override;

		virtual bool IsInputKey(Keys k) override;
		virtual void WndProc(Message% m) override;
		virtual void CreateHandle() override;
		virtual void DestroyHandle() override;

		//

		virtual void OnGLInit(Object^ obj, EventArgs^ e);
		virtual void OnGLDraw(Object^ obj, EventArgs^ e);
		virtual void OnGLResize(Object^ obj, EventArgs^ e);
		virtual void OnGLKeyDown(Object^ obj, KeyEventArgs^ e);
		virtual void OnGLMouseDown(Object^ obj, MouseEventArgs^ e);
		virtual void OnGLMouseScroll(Object^ obj, MouseEventArgs^ e);
		virtual void OnGLMouseMove(Object^ obj, MouseEventArgs^ e);
	};
}
