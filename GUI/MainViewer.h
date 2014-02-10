#pragma once

#include <GL/glew.h>
#include <GL/wglew.h>



#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")

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
	protected: 

	private:

		void GLInit(Object^ sender, EventArgs^ e)
		{
			glClearColor(.1f, .2f, .3f, 1.f);
			//AdvGfxCore::Init();
		}

		void GLDraw(Object^ sender, EventArgs^ e)
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			//AdvGfxCore::Draw();
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
