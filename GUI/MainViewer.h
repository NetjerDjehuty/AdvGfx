#pragma once

#include <GL/glew.h>
#include <GL/wglew.h>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")

#include "../AdvGfx/AdvGfx.h"

namespace GUI {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MainViewer
	/// </summary>
	public ref class MainViewer : public System::Windows::Forms::Form
	{
	public:
		MainViewer(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//

			this->glViewer1 = gcnew GLViewer::GLViewer();
			this->glViewer1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer1->Panel1->Controls->Add(this->glViewer1);


			this->glViewer1->GLInit += gcnew System::EventHandler(this, &MainViewer::GLInit);
			this->glViewer1->GLDraw += gcnew System::EventHandler(this, &MainViewer::GLDraw);
			this->glViewer1->GLKeyDown += gcnew KeyEventHandler(this, &MainViewer::GLKeyDown);
			this->glViewer1->GLMouseDown += gcnew MouseEventHandler(this, &MainViewer::GLMouseDown);
			this->glViewer1->GLMouseScroll += gcnew MouseEventHandler(this, &MainViewer::GLMouseScroll);
			this->glViewer1->GLMouseMove += gcnew MouseEventHandler(this, &MainViewer::GLMouseMove);
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MainViewer()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::SplitContainer^  splitContainer1;
	private: GLViewer::GLViewer^ glViewer1;
	System::Drawing::Point oldLoc;
	private:
		void GLInit(Object^ sender, EventArgs^ e)
		{
			oldLoc = System::Drawing::Point(0.0, 0.0);
			AdvGfxCore::Init();
		}

		void GLDraw(Object^ sender, EventArgs^ e)
		{
			AdvGfxCore::Draw();
		}

		void GLKeyDown(Object^ sender, KeyEventArgs^ e)
		{
			if(e->KeyCode == Keys::Left || e->KeyCode == Keys::A)
				MoveCamera(-10.f, .0f, .0f);
			if(e->KeyCode == Keys::Right || e->KeyCode == Keys::D)
				MoveCamera(10.f, .0f, .0f);
			if(e->KeyCode == Keys::Up || e->KeyCode == Keys::W)
				MoveCamera(.0f, 10.f, .0f);
			if(e->KeyCode == Keys::Down || e->KeyCode == Keys::S)
				MoveCamera(.0f, -10.f, .0f);
			if(e->KeyCode == Keys::R)
				ResetCamera();
			if(e->KeyCode == Keys::Subtract)
				MoveCamera(.0f, .0f, -10.f);
			if(e->KeyCode == Keys::Add)
				MoveCamera(.0f, .0f, 10.f);
		}

		void GLMouseDown(Object^ sender, MouseEventArgs^ e)
		{
			auto oldLocation = e->Location;
			Console::WriteLine("Mouse event");
			switch(e->Button)
			{
			case System::Windows::Forms::MouseButtons::Left:
				Console::WriteLine("left button");
				break;
			case System::Windows::Forms::MouseButtons::Right:
				Console::WriteLine("Right button");
				break;
			case System::Windows::Forms::MouseButtons::Middle:
				Console::WriteLine("Middle button");
				break;
			default:
				Console::WriteLine("Other buttton; {0}", e->Button.ToString());
			}
		}

		void GLMouseScroll(Object^ sender, MouseEventArgs^ e)
		{
			Console::WriteLine("Scrolled? Yup {0} degree", e->Delta);
			if(e->Delta < 0)
				MoveCamera(.0f,.0f,-.1f);
			else
				MoveCamera(.0f,.0f,.1f);
		}

		void GLMouseMove(Object^ sender, MouseEventArgs^ e)
		{
			System::Drawing::Point newLoc = e->Location;
			if(e->Button == System::Windows::Forms::MouseButtons::Left)
			{
				int x = (oldLoc.X - newLoc.X);
				int y = (oldLoc.Y - newLoc.Y);
				AdvGfxCore::RotateCamera(x, y);
			}
			oldLoc = newLoc;
		}


		protected: 
			void MoveCamera(float x, float y, float z)
			{
				AdvGfxCore::MoveCamera(x,y,z);
			}

		protected:
			void ResetCamera()
			{
				AdvGfxCore::ResetCamera();
			}

		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->splitContainer1 = (gcnew System::Windows::Forms::SplitContainer());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->splitContainer1))->BeginInit();
			this->splitContainer1->SuspendLayout();
			this->SuspendLayout();
			// 
			// splitContainer1
			// 
			this->splitContainer1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer1->Location = System::Drawing::Point(0, 0);
			this->splitContainer1->Name = L"splitContainer1";
			this->splitContainer1->Size = System::Drawing::Size(675, 503);
			this->splitContainer1->SplitterDistance = 555;
			this->splitContainer1->TabIndex = 0;
			// 
			// MainViewer
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(675, 503);
			this->Controls->Add(this->splitContainer1);
			this->Name = L"MainViewer";
			this->Text = L"MainViewer";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->splitContainer1))->EndInit();
			this->splitContainer1->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion
	};
}
