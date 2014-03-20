#pragma once

#include <GL/glew.h>
#include <GL/wglew.h>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")

#include "../AdvGfx/AdvGfx.h"


#include <msclr\marshal_cppstd.h>

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
			this->glViewer1->GLResize += gcnew System::EventHandler(this, &MainViewer::GLResize);
			this->glViewer1->GLKeyDown += gcnew KeyEventHandler(this, &MainViewer::GLKeyDown);
			this->glViewer1->GLKeyUp += gcnew KeyEventHandler(this, &MainViewer::GLKeyUp);
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
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::OpenFileDialog^  openFileDialog1;
			 System::Drawing::Point oldLoc;
	private:
		void GLInit(Object^ sender, EventArgs^ e)
		{
			oldLoc = System::Drawing::Point(0.0, 0.0);
			AdvGfxCore::Init(this->Width, this->Height);
		}

		void GLDraw(Object^ sender, EventArgs^ e)
		{
			AdvGfxCore::Draw();
		}

		void GLResize(Object^ sender, EventArgs^ e)
		{
			AdvGfxCore::Resize(glViewer1->Width, glViewer1->Height);
		}

		void GLKeyDown(Object^ sender, KeyEventArgs^ e)
		{
			switch (e->KeyCode)
			{
			case Keys::Left :
			case Keys::A:
				AdvGfxCore::MoveCamera(AdvGfxCore::Movement::Left, false);
				break;
			case Keys::Right:
			case Keys::D:
				AdvGfxCore::MoveCamera(AdvGfxCore::Movement::Right, false);
				break;
			case Keys::Up:
			case Keys::W:
				AdvGfxCore::MoveCamera(AdvGfxCore::Movement::Forward, false);
				break;
			case Keys::Down:
			case Keys::S:
				AdvGfxCore::MoveCamera(AdvGfxCore::Movement::Backward, false);
				break;
			case Keys::R:
				AdvGfxCore::ResetCamera();
				break;
			case Keys::Subtract:
			case Keys::Z:
				AdvGfxCore::MoveCamera(AdvGfxCore::Movement::Down, false);
				break;
			case Keys::Add:
			case Keys::Q:
				AdvGfxCore::MoveCamera(AdvGfxCore::Movement::Up, false);
				break;
			}
		}

		void GLKeyUp(Object^ sender, KeyEventArgs^ e)
		{

			switch (e->KeyCode)
			{
			case Keys::Left:
			case Keys::A:
				AdvGfxCore::MoveCamera(AdvGfxCore::Movement::Left, true);
				break;
			case Keys::Right:
			case Keys::D:
				AdvGfxCore::MoveCamera(AdvGfxCore::Movement::Right, true);
				break;
			case Keys::Up:
			case Keys::W:
				AdvGfxCore::MoveCamera(AdvGfxCore::Movement::Forward, true);
				break;
			case Keys::Down:
			case Keys::S:
				AdvGfxCore::MoveCamera(AdvGfxCore::Movement::Backward, true);
				break;
			case Keys::R:
				AdvGfxCore::ResetCamera();
				break;
			case Keys::Subtract:
			case Keys::Z:
				AdvGfxCore::MoveCamera(AdvGfxCore::Movement::Down, true);
				break;
			case Keys::Add:
			case Keys::Q:
				AdvGfxCore::MoveCamera(AdvGfxCore::Movement::Up, true);
				break;
			}
		}

		void GLMouseDown(Object^ sender, MouseEventArgs^ e)
		{
			auto oldLocation = e->Location;
			switch(e->Button)
			{
			case System::Windows::Forms::MouseButtons::Left:
				break;
			case System::Windows::Forms::MouseButtons::Right:
				break;
			case System::Windows::Forms::MouseButtons::Middle:
				break;
			default:
				break;
			}
		}

		void GLMouseScroll(Object^ sender, MouseEventArgs^ e)
		{
			if(e->Delta < 0)
				AdvGfxCore::MoveCamera(.0f,.0f,-.1f);
			else
				AdvGfxCore::MoveCamera(.0f,.0f,.1f);
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
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->splitContainer1))->BeginInit();
			this->splitContainer1->Panel2->SuspendLayout();
			this->splitContainer1->SuspendLayout();
			this->SuspendLayout();
			// 
			// splitContainer1
			// 
			this->splitContainer1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer1->Location = System::Drawing::Point(0, 0);
			this->splitContainer1->Name = L"splitContainer1";
			// 
			// splitContainer1.Panel2
			// 
			this->splitContainer1->Panel2->Controls->Add(this->button1);
			this->splitContainer1->Size = System::Drawing::Size(1482, 720);
			this->splitContainer1->SplitterDistance = 1280;
			this->splitContainer1->TabIndex = 0;
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(85, 685);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(101, 23);
			this->button1->TabIndex = 0;
			this->button1->Text = L"Load Object";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MainViewer::button1_Click);
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->Filter = L"Obj file (*.obj)|*.obj";
			// 
			// MainViewer
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1482, 720);
			this->Controls->Add(this->splitContainer1);
			this->Name = L"MainViewer";
			this->Text = L"MainViewer";
			this->splitContainer1->Panel2->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->splitContainer1))->EndInit();
			this->splitContainer1->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion
	private: 

		void LoadEvent(Object^ sender, EventArgs^ e)
		{
			std::string path = msclr::interop::marshal_as<std::string>(openFileDialog1->SafeFileName);
			AdvGfxCore::load(path.c_str());
		}

		System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {

			System::Windows::Forms::DialogResult result = openFileDialog1->ShowDialog();
			if(result == System::Windows::Forms::DialogResult::OK)
			{				
				this->glViewer1->QueueActions(gcnew EventHandler(this, &GUI::MainViewer::LoadEvent), this, e);
				this->glViewer1->Focus();
				// Reset Camera
				SendKeys::Send("R");
			}
		}
	};
}
