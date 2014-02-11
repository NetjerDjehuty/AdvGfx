// This is the main DLL file.

#include "stdafx.h"

#include "GLViewer.h"

namespace GLViewer
{
	GLViewer::GLViewer() : UserControl(), _initialized(false), ActionQueue(gcnew Queue<Tuple<Delegate^, array<Object^>^>^>()),
		_terminated(gcnew ManualResetEvent(false)), _errMsg(""),
		_queueLock(gcnew Object())
	{
		InitializeComponent();

		//SetStyle(ControlStyles::ResizeRedraw, false);
		SetStyle(ControlStyles::UserPaint, false);

		// Do not create renderingthread in design mode!
		if (DesignMode)
		{
			_errMsg = "Design Mode";
			return;
		}

		CreateRThread();
	}

	GLViewer::~GLViewer()
	{
		// terminate the rendering thread
		StopRThread();

		if (components)
		{
			delete components;
		}
	}

	void GLViewer::CreateRThread()
	{
		_renderThread = gcnew Thread(gcnew ParameterizedThreadStart(this, &GLViewer::Execute));
		// TODO: experiment with thread priority
		_renderThread->Priority = ThreadPriority::AboveNormal;
	}

	void GLViewer::StartRThread()
	{
		if (_renderThread)
			_renderThread->Start(Handle);
	}

	void GLViewer::StopRThread()
	{
		// Signal rendering thread to terminate
		if (_terminated)
			_terminated->Set();

		// wait for thread to finish
		if (_renderThread)
			_renderThread->Join();
	}

	void GLViewer::InitContext(HWND WindowHandle)
	{
		// We want to create an OpenGL 3.0 or greater context (preferably 4.0 or greater). For this we first need to create a dummy context.

		_hdc = GetDC(WindowHandle);
		_hrc = NULL;

		PIXELFORMATDESCRIPTOR pfd =
		{
			sizeof(PIXELFORMATDESCRIPTOR),
			1,
			PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,		//Flags
			PFD_TYPE_RGBA,													//The kind of framebuffer. RGBA or palette.
			32,																//Colordepth of the framebuffer.
			0, 0, 0, 0, 0, 0,
			0,
			0,
			0,
			0, 0, 0, 0,
			24,																//Number of bits for the depthbuffer
			8,																//Number of bits for the stencilbuffer
			0,																//Number of Aux buffers in the framebuffer.
			PFD_MAIN_PLANE,
			0,
			0, 0, 0
		};


		int nPixelFormat = ChoosePixelFormat(_hdc, &pfd);
		SetPixelFormat(_hdc, nPixelFormat, &pfd);

		HGLRC tempContext = wglCreateContext(_hdc);

		// Using our dummy context, we can load the WGL_ARB_CREATE_CONTEXT extension using GLEW
		wglMakeCurrent(_hdc, tempContext);

		GLenum err = glewInit();
		if (err != GLEW_OK)
		{
			ReleaseContext(WindowHandle);
			wglDeleteContext(tempContext);
			_errMsg = "Could not initialize GLEW";

			return;
		}

		// attributes for our context:
		int attribs[] =
		{
			WGL_CONTEXT_MAJOR_VERSION_ARB, 4,									// Major version 4
			WGL_CONTEXT_MINOR_VERSION_ARB, 2,									// Minor version 2
			WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,		// We want to use the core profile
			WGL_CONTEXT_FLAGS_ARB, 0,											// No flags for now, set to debug for debug context
			0
		};

		if (wglewIsSupported("WGL_ARB_create_context"))
		{
			// here we create our actual context
			_hrc = wglCreateContextAttribsARB(_hdc, 0, attribs);
			wglMakeCurrent(NULL, NULL);
			wglDeleteContext(tempContext);
			wglMakeCurrent(_hdc, _hrc);
		}
		else
		{
			// we cannot create our context
			ReleaseContext(WindowHandle);
			wglDeleteContext(tempContext);
			_errMsg = "Could not create GL context";

			return;
		}

		// initialization was succesfull
		_initialized = true;
	}

	void GLViewer::ReleaseContext(HWND WindowHandle)
	{
		// deactivate the viewer
		_initialized = false;

		// unbind the context
		wglMakeCurrent(NULL, NULL);

		if (_hrc)
			wglDeleteContext(_hrc);

		if (_hdc)
			ReleaseDC(WindowHandle, _hdc);
	}

	void GLViewer::Refresh()
	{
		// suppress if we are in design mode or if the viewer has not been initialized
		if (DesignMode || !_initialized)
			__super::Refresh();
	}

	void GLViewer::OnPaint(PaintEventArgs^ e)
	{
		// suppress if we are in design mode or if the viewer has not been initialized
		if (DesignMode || !_initialized)
		{
			__super::OnPaint(e);

			Pen^ pen = gcnew Pen(Color::Red, 1);
			e->Graphics->DrawLine(pen, 0, 0, Width - 1, Height - 1);
			e->Graphics->DrawLine(pen, Width - 1, 0, 0, Height - 1);
			e->Graphics->DrawRectangle(pen, 0, 0, Width - 1, Height - 1);

			SizeF s = e->Graphics->MeasureString(_errMsg, Font);
			PointF p = PointF(Width / 2.0F - s.Width / 2.0F, Height / 2.0F - s.Height / 2.0F);
			e->Graphics->FillRectangle(gcnew SolidBrush(BackColor), p.X - 1.0F, p.Y - 1.0F, s.Width + 1.0F, s.Height + 1.0F);
			e->Graphics->DrawString(_errMsg, Font, gcnew SolidBrush(ForeColor), p);
		}

	}

	void GLViewer::OnPaintBackground(PaintEventArgs^ e)
	{
		// suppress if we are in design mode or if the viewer has not been initialized
		if (DesignMode || !_initialized)
			__super::OnPaintBackground(e);
	}

	void GLViewer::Execute(Object^ WindowHandle)
	{
		HWND hwnd = (HWND)((IntPtr)WindowHandle).ToPointer();

		// initialize the context first
		InitContext(hwnd);

		// check to see if initialization was succesfull
		if (!_initialized)
			return;

		QueueActions(gcnew EventHandler(this, &GLViewer::OnGLInit), this, EventArgs::Empty);

		while (!_terminated->WaitOne(0))
		{
			// queue render action
			QueueActions(gcnew EventHandler(this, &GLViewer::OnGLDraw), this, EventArgs::Empty);

			RunActions();
		}

		// We are done with the GL rendering: release the context
		ReleaseContext(hwnd);
	}

	void GLViewer::QueueActions(Delegate^ AsyncAction, ... array<Object^>^ Args)
	{
		Monitor::Enter(_queueLock);
		try
		{
			Action^ ActionItem = Tuple::Create(AsyncAction, Args);
			ActionQueue->Enqueue(ActionItem);
		}
		finally
		{
			Monitor::Exit(_queueLock);
		}
	}

	void GLViewer::RunActions()
	{
		// copy queue and clear

		Queue<Action^>^ copy;
		Monitor::Enter(_queueLock);
		try
		{
			copy = gcnew Queue<Action^>(ActionQueue);
			ActionQueue->Clear();
		}
		finally
		{
			Monitor::Exit(_queueLock);
		}

		// perform actions
		while (copy->Count > 0)
		{
			Action^ action = copy->Dequeue();
			action->Item1->DynamicInvoke(action->Item2);
		}
	}

	void GLViewer::OnResize(EventArgs^ e)
	{
		__super::OnResize(e);

		if (DesignMode || !_initialized)
			return;

		QueueActions(gcnew EventHandler(this, &GLViewer::OnGLResize), this, e);
	}

	void GLViewer::OnLoad(EventArgs^ e)
	{
		StartRThread();
	}

	void GLViewer::CreateHandle()
	{
		__super::CreateHandle();

		if (RecreatingHandle)
			CreateRThread();
	}

	void GLViewer::OnKeyDown(KeyEventArgs^ e)
	{
		if(DesignMode || !_initialized)
			return;

		QueueActions(gcnew KeyEventHandler(this, &GLViewer::OnGLKeyDown), this, e);
	}

	void GLViewer::OnMouseDown(MouseEventArgs^ e)
	{
		if(DesignMode || !_initialized)
			return;

		QueueActions(gcnew MouseEventHandler(this, &GLViewer::OnGLMouseDown), this, e);
	}

	void GLViewer::DestroyHandle()
	{
		if (RecreatingHandle)
			StopRThread();

		__super::DestroyHandle();
	}

	void GLViewer::WndProc(Message% m)
	{
		switch (m.Msg)
		{
		case WM_PAINT:
			OnPaint(gcnew PaintEventArgs(Graphics::FromHwnd(Handle), ClientRectangle));
			break;
		case WM_ERASEBKGND:
			OnPaintBackground(gcnew PaintEventArgs(Graphics::FromHwnd(Handle), ClientRectangle));
			break;
		}

		__super::WndProc(m);
	}


	void GLViewer::OnGLDraw(Object^ obj, EventArgs^ e)
	{
		// Render the scene
		GLDraw(this, e);
		SwapBackBuffers();
	}

	void GLViewer::OnGLResize(Object^ obj, EventArgs^ e)
	{
		glViewport(0, 0, Width, Height);
	}

	void GLViewer::OnGLInit(Object^ obj, EventArgs^ e)
	{
		GLInit(this, e);
	}

	void GLViewer::OnGLKeyDown(Object^ obj, KeyEventArgs^ e)
	{
		GLKeyDown(this, e);
	}

	void GLViewer::OnGLMouseDown(Object^ obj, MouseEventArgs^ e)
	{
		GLMouseDown(this, e);
	}

	void GLViewer::SwapBackBuffers()
	{
		if (_initialized)
			SwapBuffers(_hdc);
	}
}