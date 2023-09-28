#pragma once
#//include "DotNetUtilities.h"
//#include "object.h"
#include "MeshObject.h"
#include "DrawModelShader.h"
#include <ResourcePath.h>
#include <AntTweakBar/AntTweakBar.h>
#include "PickingShader.h"
#include "DrawPickingFaceShader.h"
#include "PickingTexture.h"
#include <ViewManager.h>





glm::mat4 ProjectionMatrix;
glm::mat4 ViewMatrix;




//bool reTime = false;
//float countTime = 0;



int windowWidth = 1200;
int windowHeight = 600;
int windowWidth2 = 1200;
int windowHeight2 = 600;
float uvRotateAngle = 0.0;
float prevUVRotateAngle = 0.0;
GLuint currentFaceID = 0;
bool drawTexture = false;

GLuint textureID;
bool isRightButtonPress = false;
float			aspect;
float aspect2;
std::vector <std::string> paths;

MeshObject model;
DrawModelShader drawModelShader;
PickingShader pickingShader;
DrawPickingFaceShader drawPickingFaceShader;
PickingTexture pickingTexture;
ViewManager		meshWindowCam;


enum ModeSelection
{
	ADD_FACE,
	SMALL,
	MEDIUM,
	LARGE,
	DEL_FACE,
	DELETE_SMALL,
	DELETE_MEDIUM,
	DELETE_LARGE,
	EXPAND_FACE,
	DIMINISH_FACE,
};
ModeSelection modeSelection = ADD_FACE;





namespace shader_example {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// MyForm 的摘要
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO:  在此加入建構函式程式碼
			//
		}

	protected:
		/// <summary>
		/// 清除任何使用中的資源。
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}

	private: System::Windows::Forms::Timer^ timer1;
	private: System::Windows::Forms::ListBox^ Mode;
	private: System::Windows::Forms::CheckBox^ drawTextureCheck;


	private: System::Windows::Forms::Button^ ParameterizationButton;

	private: System::Windows::Forms::ListBox^ TextureBox;

	private: HKOGLPanel::HKOGLPanelControl^ hkoglPanelControl1;


	private: System::Windows::Forms::OpenFileDialog^ openFileDialog;


	private: System::Windows::Forms::SaveFileDialog^ saveFileDialog;
	private: System::Windows::Forms::Button^ RecoveryButton;
	private: System::Windows::Forms::Button^ NextButton;


	private: System::Windows::Forms::OpenFileDialog^ AddTextureDialog;
	private: System::Windows::Forms::MenuStrip^ menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^ textureToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ addToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ deleteToolStripMenuItem;
	private: System::Windows::Forms::ListBox^ LogWindow;
	private: System::Windows::Forms::ToolStripMenuItem^ fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ openToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ saveToolStripMenuItem;
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Label^ label2;







	private: System::ComponentModel::IContainer^ components;
	protected:

	private:
		/// <summary>
		/// 設計工具所需的變數。
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// 此為設計工具支援所需的方法 - 請勿使用程式碼編輯器修改
		/// 這個方法的內容。
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			HKOGLPanel::HKCOGLPanelCameraSetting^ hkcoglPanelCameraSetting1 = (gcnew HKOGLPanel::HKCOGLPanelCameraSetting());
			HKOGLPanel::HKCOGLPanelPixelFormat^ hkcoglPanelPixelFormat1 = (gcnew HKOGLPanel::HKCOGLPanelPixelFormat());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->Mode = (gcnew System::Windows::Forms::ListBox());
			this->drawTextureCheck = (gcnew System::Windows::Forms::CheckBox());
			this->ParameterizationButton = (gcnew System::Windows::Forms::Button());
			this->TextureBox = (gcnew System::Windows::Forms::ListBox());
			this->hkoglPanelControl1 = (gcnew HKOGLPanel::HKOGLPanelControl());
			this->openFileDialog = (gcnew System::Windows::Forms::OpenFileDialog());
			this->saveFileDialog = (gcnew System::Windows::Forms::SaveFileDialog());
			this->RecoveryButton = (gcnew System::Windows::Forms::Button());
			this->NextButton = (gcnew System::Windows::Forms::Button());
			this->AddTextureDialog = (gcnew System::Windows::Forms::OpenFileDialog());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->saveToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->textureToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->addToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->deleteToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->LogWindow = (gcnew System::Windows::Forms::ListBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->menuStrip1->SuspendLayout();
			this->SuspendLayout();
			// 
			// timer1
			// 
			this->timer1->Enabled = true;
			this->timer1->Interval = 10;
			this->timer1->Tick += gcnew System::EventHandler(this, &MyForm::timer1_Tick);
			// 
			// Mode
			// 
			this->Mode->Font = (gcnew System::Drawing::Font(L"新細明體", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(136)));
			this->Mode->FormattingEnabled = true;
			this->Mode->ItemHeight = 21;
			this->Mode->Items->AddRange(gcnew cli::array< System::Object^  >(10) {
				L"Add Face", L"Paint Brush(Small)", L"Paint Brush(Medium)",
					L"Paint Brush(Large)", L"Delete Face", L"Erase Brush(Small)", L"Erase Brush(Medium)", L"Erase Brush(Large)", L"Expand Face",
					L"Diminish Face"
			});
			this->Mode->Location = System::Drawing::Point(12, 49);
			this->Mode->Name = L"Mode";
			this->Mode->Size = System::Drawing::Size(341, 109);
			this->Mode->TabIndex = 1;
			this->Mode->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::Mode_SelectedIndexChanged);
			// 
			// drawTextureCheck
			// 
			this->drawTextureCheck->AutoSize = true;
			this->drawTextureCheck->Font = (gcnew System::Drawing::Font(L"新細明體", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(136)));
			this->drawTextureCheck->Location = System::Drawing::Point(29, 355);
			this->drawTextureCheck->Name = L"drawTextureCheck";
			this->drawTextureCheck->Size = System::Drawing::Size(140, 25);
			this->drawTextureCheck->TabIndex = 2;
			this->drawTextureCheck->Text = L"Draw Texture";
			this->drawTextureCheck->UseVisualStyleBackColor = true;
			this->drawTextureCheck->CheckedChanged += gcnew System::EventHandler(this, &MyForm::drawTextureBox_CheckedChanged);
			// 
			// ParameterizationButton
			// 
			this->ParameterizationButton->Font = (gcnew System::Drawing::Font(L"新細明體", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(136)));
			this->ParameterizationButton->Location = System::Drawing::Point(12, 388);
			this->ParameterizationButton->Name = L"ParameterizationButton";
			this->ParameterizationButton->Size = System::Drawing::Size(166, 32);
			this->ParameterizationButton->TabIndex = 4;
			this->ParameterizationButton->Text = L"Parameterization";
			this->ParameterizationButton->UseVisualStyleBackColor = true;
			this->ParameterizationButton->Click += gcnew System::EventHandler(this, &MyForm::ParameterizationButton_Click);
			// 
			// TextureBox
			// 
			this->TextureBox->Font = (gcnew System::Drawing::Font(L"新細明體", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(136)));
			this->TextureBox->FormattingEnabled = true;
			this->TextureBox->ItemHeight = 21;
			this->TextureBox->Items->AddRange(gcnew cli::array< System::Object^  >(23) {
				L"zebra", L"turtle", L"eye", L"bloodeye", L"alligator",
					L"giraffe", L"tiger", L"leopard", L"fur", L"lava", L"skin", L"dinosaur", L"nose", L"hair", L"brick", L"stone", L"leaf", L"coral",
					L"peacock", L"ocean", L"shell", L"dead leaves", L"trunk"
			});
			this->TextureBox->Location = System::Drawing::Point(12, 192);
			this->TextureBox->Name = L"TextureBox";
			this->TextureBox->Size = System::Drawing::Size(341, 151);
			this->TextureBox->TabIndex = 5;
			this->TextureBox->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::TextureBox_SelectedIndexChanged);
			// 
			// hkoglPanelControl1
			// 
			this->hkoglPanelControl1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			hkcoglPanelCameraSetting1->Far = 1000;
			hkcoglPanelCameraSetting1->Fov = 45;
			hkcoglPanelCameraSetting1->Near = -1000;
			hkcoglPanelCameraSetting1->Type = HKOGLPanel::HKCOGLPanelCameraSetting::CAMERATYPE::ORTHOGRAPHIC;
			this->hkoglPanelControl1->Camera_Setting = hkcoglPanelCameraSetting1;
			this->hkoglPanelControl1->Location = System::Drawing::Point(359, 27);
			this->hkoglPanelControl1->Name = L"hkoglPanelControl1";
			hkcoglPanelPixelFormat1->Accumu_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
			hkcoglPanelPixelFormat1->Alpha_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
			hkcoglPanelPixelFormat1->Stencil_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
			this->hkoglPanelControl1->Pixel_Format = hkcoglPanelPixelFormat1;
			this->hkoglPanelControl1->Size = System::Drawing::Size(344, 476);
			this->hkoglPanelControl1->TabIndex = 0;
			this->hkoglPanelControl1->Load += gcnew System::EventHandler(this, &MyForm::hkoglPanelControl1_Load);
			this->hkoglPanelControl1->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MyForm::hkoglPanelControl1_Paint);
			this->hkoglPanelControl1->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::hkoglPanelControl1_MouseDown);
			this->hkoglPanelControl1->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::hkoglPanelControl1_MouseMove);
			this->hkoglPanelControl1->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::hkoglPanelControl1_MouseUp);
			this->hkoglPanelControl1->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::hkoglPanelControl1_MouseWheel);
			// 
			// openFileDialog
			// 
			this->openFileDialog->FileOk += gcnew System::ComponentModel::CancelEventHandler(this, &MyForm::loadModelDialog_FileOk);
			// 
			// saveFileDialog
			// 
			this->saveFileDialog->FileOk += gcnew System::ComponentModel::CancelEventHandler(this, &MyForm::saveFileDialog_FileOk);
			// 
			// RecoveryButton
			// 
			this->RecoveryButton->Font = (gcnew System::Drawing::Font(L"新細明體", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(136)));
			this->RecoveryButton->Location = System::Drawing::Point(187, 351);
			this->RecoveryButton->Name = L"RecoveryButton";
			this->RecoveryButton->Size = System::Drawing::Size(166, 32);
			this->RecoveryButton->TabIndex = 9;
			this->RecoveryButton->Text = L"Undo";
			this->RecoveryButton->UseVisualStyleBackColor = true;
			this->RecoveryButton->Click += gcnew System::EventHandler(this, &MyForm::RecoveryButton_Click);
			// 
			// NextButton
			// 
			this->NextButton->Font = (gcnew System::Drawing::Font(L"新細明體", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(136)));
			this->NextButton->Location = System::Drawing::Point(188, 388);
			this->NextButton->Name = L"NextButton";
			this->NextButton->Size = System::Drawing::Size(166, 32);
			this->NextButton->TabIndex = 10;
			this->NextButton->Text = L"Redo";
			this->NextButton->UseVisualStyleBackColor = true;
			this->NextButton->Click += gcnew System::EventHandler(this, &MyForm::NextButton_Click);
			// 
			// AddTextureDialog
			// 
			this->AddTextureDialog->FileOk += gcnew System::ComponentModel::CancelEventHandler(this, &MyForm::AddTextureDialog_FileOk);
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->fileToolStripMenuItem,
					this->textureToolStripMenuItem
			});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(703, 28);
			this->menuStrip1->TabIndex = 13;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->openToolStripMenuItem,
					this->saveToolStripMenuItem
			});
			this->fileToolStripMenuItem->Font = (gcnew System::Drawing::Font(L"Microsoft JhengHei UI", 12));
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(46, 24);
			this->fileToolStripMenuItem->Text = L"File";
			// 
			// openToolStripMenuItem
			// 
			this->openToolStripMenuItem->Name = L"openToolStripMenuItem";
			this->openToolStripMenuItem->Size = System::Drawing::Size(115, 24);
			this->openToolStripMenuItem->Text = L"Load";
			this->openToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::openToolStripMenuItem_Click);
			// 
			// saveToolStripMenuItem
			// 
			this->saveToolStripMenuItem->Name = L"saveToolStripMenuItem";
			this->saveToolStripMenuItem->Size = System::Drawing::Size(115, 24);
			this->saveToolStripMenuItem->Text = L"Save";
			this->saveToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::saveToolStripMenuItem_Click);
			// 
			// textureToolStripMenuItem
			// 
			this->textureToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->addToolStripMenuItem,
					this->deleteToolStripMenuItem
			});
			this->textureToolStripMenuItem->Font = (gcnew System::Drawing::Font(L"Microsoft JhengHei UI", 12));
			this->textureToolStripMenuItem->Name = L"textureToolStripMenuItem";
			this->textureToolStripMenuItem->Size = System::Drawing::Size(78, 24);
			this->textureToolStripMenuItem->Text = L"Texture";
			// 
			// addToolStripMenuItem
			// 
			this->addToolStripMenuItem->Name = L"addToolStripMenuItem";
			this->addToolStripMenuItem->Size = System::Drawing::Size(127, 24);
			this->addToolStripMenuItem->Text = L"Add";
			this->addToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::addToolStripMenuItem_Click);
			// 
			// deleteToolStripMenuItem
			// 
			this->deleteToolStripMenuItem->Name = L"deleteToolStripMenuItem";
			this->deleteToolStripMenuItem->Size = System::Drawing::Size(127, 24);
			this->deleteToolStripMenuItem->Text = L"Delete";
			this->deleteToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::deleteToolStripMenuItem_Click);
			// 
			// LogWindow
			// 
			this->LogWindow->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left));
			this->LogWindow->BackColor = System::Drawing::SystemColors::InfoText;
			this->LogWindow->Font = (gcnew System::Drawing::Font(L"新細明體", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(136)));
			this->LogWindow->ForeColor = System::Drawing::SystemColors::Window;
			this->LogWindow->FormattingEnabled = true;
			this->LogWindow->ItemHeight = 16;
			this->LogWindow->Location = System::Drawing::Point(12, 426);
			this->LogWindow->Name = L"LogWindow";
			this->LogWindow->ScrollAlwaysVisible = true;
			this->LogWindow->SelectionMode = System::Windows::Forms::SelectionMode::None;
			this->LogWindow->Size = System::Drawing::Size(341, 68);
			this->LogWindow->TabIndex = 14;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"新細明體", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(136)));
			this->label1->Location = System::Drawing::Point(8, 27);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(100, 19);
			this->label1->TabIndex = 15;
			this->label1->Text = L"Select Mode";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Font = (gcnew System::Drawing::Font(L"新細明體", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(136)));
			this->label2->Location = System::Drawing::Point(12, 170);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(65, 19);
			this->label2->TabIndex = 16;
			this->label2->Text = L"Texture";
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(703, 506);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->LogWindow);
			this->Controls->Add(this->NextButton);
			this->Controls->Add(this->RecoveryButton);
			this->Controls->Add(this->hkoglPanelControl1);
			this->Controls->Add(this->ParameterizationButton);
			this->Controls->Add(this->drawTextureCheck);
			this->Controls->Add(this->TextureBox);
			this->Controls->Add(this->Mode);
			this->Controls->Add(this->menuStrip1);
			this->Name = L"MyForm";
			this->Text = L"Form";
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

		//========================================================================================================
		//========================================================================================================
		//上面是windows form

	
	private: System::Void hkoglPanelControl1_Load(System::Object^ sender, System::EventArgs^ e) {

		//Init OpenGL

		glEnable(GL_TEXTURE_2D);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_POINT_SMOOTH);


		printf("Vendor: %s\n", glGetString(GL_VENDOR));
		printf("Renderer: %s\n", glGetString(GL_RENDERER));
		printf("Version: %s\n", glGetString(GL_VERSION));
		printf("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

		//glew初始
		GLenum res = glewInit();
		if (res != GLEW_OK) {
			fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		}
		glClearColor(0, 0, 0, 1);


		//物件初始
		windowHeight = hkoglPanelControl1->Size.Height;
		windowWidth = hkoglPanelControl1->Size.Width;


		ResourcePath::shaderPath = "../shader_example/Shader/";
		ResourcePath::imagePath = "../shader_example/Imgs/";
		ResourcePath::modelPath = "../shader_example/Model/armadillo.obj";
		ResourcePath::savePath = "../shader_example/Save/";


		std::string path = ResourcePath::imagePath + "zebra.png"; //0 肚子
		paths.push_back(path);
		path = ResourcePath::imagePath + "turtle.jpg"; //1 龜殼
		paths.push_back(path);
		path = ResourcePath::imagePath + "eye.jpg"; //2 左眼
		paths.push_back(path);
		path = ResourcePath::imagePath + "bloodeye.png"; //3 右眼
		paths.push_back(path);
		path = ResourcePath::imagePath + "alligator.jpg"; //4 右腳
		paths.push_back(path);
		path = ResourcePath::imagePath + "giraffe.jpg"; //5 左腳
		paths.push_back(path);
		path = ResourcePath::imagePath + "tiger.png"; //6 右手
		paths.push_back(path);
		path = ResourcePath::imagePath + "leopard.jpg"; //7 左手
		paths.push_back(path);
		path = ResourcePath::imagePath + "fur.jpg";  //8 右耳
		paths.push_back(path);
		path = ResourcePath::imagePath + "lava.jpg"; //9 左耳
		paths.push_back(path);
		path = ResourcePath::imagePath + "skin.jpg"; // 10 臉
		paths.push_back(path);
		path = ResourcePath::imagePath + "dinosaur.jpg"; // 11 尾巴
		paths.push_back(path);
		path = ResourcePath::imagePath + "galaxy.jpg"; // 12 鼻子
		paths.push_back(path);
		path = ResourcePath::imagePath + "hair.jpg"; //13 hair
		paths.push_back(path);
		path = ResourcePath::imagePath + "brick.jpg"; //14 右肩
		paths.push_back(path);
		path = ResourcePath::imagePath + "stone.jpg"; //15 左肩
		paths.push_back(path);
		path = ResourcePath::imagePath + "leaf.jpg"; //16 右前腿
		paths.push_back(path);
		path = ResourcePath::imagePath + "coral.jpg"; //17 右後腿
		paths.push_back(path);
		path = ResourcePath::imagePath + "Peacock.jpg"; //18 右後腿
		paths.push_back(path);
		path = ResourcePath::imagePath + "ocean.jpg"; //19 右後腿
		paths.push_back(path);
		path = ResourcePath::imagePath + "shell.jpg"; //20 胯下
		paths.push_back(path);
		path = ResourcePath::imagePath + "deadLeaves.jpg"; //21 後背下方
		paths.push_back(path);
		path = ResourcePath::imagePath + "trunk.png"; //22 左腰
		paths.push_back(path);


		if (model.Init(ResourcePath::modelPath))
		{
			//顯示在LogWindow
			LogWindow->Items->Add("Load Model");
			LogWindow->TopIndex = LogWindow->Items->Count - 1;
		}
		else
		{
			//顯示在LogWindow
			LogWindow->Items->Add("Load Model Failed");
			LogWindow->TopIndex = LogWindow->Items->Count - 1;
		}
		for (int i = 0; i < paths.size(); i++)
		{
			model.AddTexture(paths[i]);
		}

		drawModelShader.Init();
		pickingShader.Init();
		drawPickingFaceShader.Init();
		pickingTexture.Init(windowWidth, windowHeight);

		meshWindowCam.SetWindowSize(windowWidth, windowHeight);
		aspect = windowWidth * 1.0f / windowHeight;


		//預設狀態
		Mode->SelectedIndex = 0;
		TextureBox->SelectedIndex = 0;


		//預設第0步為空的
		model.recordNumber = 0;
		model.record.clear();
		model.record[model.recordNumber].resize(model.textures.size());
		for (int i = 0; i < model.textures.size(); i++)
		{
			model.record[model.recordNumber][i].selectedFace = model.textures[i]->selectedFace;
			model.record[model.recordNumber][i].elemCount = model.textures[i]->elemCount;
			model.record[model.recordNumber][i].fvIDsPtr = model.textures[i]->fvIDsPtr;
		}
		model.checkNext = true;


	}

	private: System::Void hkoglPanelControl1_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e) {

		//Reshape
		windowHeight = hkoglPanelControl1->Size.Height;
		windowWidth = hkoglPanelControl1->Size.Width;
		TwWindowSize(windowWidth, windowHeight);
		glViewport(0, 0, windowWidth, windowHeight);
		aspect = windowWidth * 1.0f / windowHeight;

		meshWindowCam.SetWindowSize(windowWidth, windowHeight);
		pickingTexture.Init(windowWidth, windowHeight);

		glm::mat4 mvMat = meshWindowCam.GetViewMatrix() * meshWindowCam.GetModelMatrix();
		glm::mat4 pMat = meshWindowCam.GetProjectionMatrix(aspect);
		glViewport(0, 0, windowWidth, windowHeight);


	// write faceID+1 to framebuffer
		pickingTexture.Enable();
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		pickingShader.Enable();
		pickingShader.SetMVMat(mvMat);
		pickingShader.SetPMat(pMat);

		model.Render();

		pickingShader.Disable();
		pickingTexture.Disable();


		//draw model
		glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		drawModelShader.Enable();
		glm::mat3 normalMat = glm::transpose(glm::inverse(glm::mat3(mvMat)));

		float radian = uvRotateAngle * M_PI / 180.0f;
		glm::mat4 uvRotMat = glm::rotate(radian, glm::vec3(0.0, 0.0, 1.0));

		drawModelShader.SetWireColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		drawModelShader.SetFaceColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		drawModelShader.UseLighting(true);

		drawModelShader.DrawWireframe(!drawTexture);
		drawModelShader.SetNormalMat(normalMat);
		drawModelShader.SetMVMat(mvMat);
		drawModelShader.SetPMat(pMat);

		drawModelShader.DrawTexCoord(false);
		drawModelShader.DrawTexture(false);

		drawModelShader.SetUVRotMat(uvRotMat);
		model.Render();


		model.currentTexture = TextureBox->SelectedIndex;
		if (!drawTexture)
		{
			drawPickingFaceShader.Enable();
			drawPickingFaceShader.SetMVMat(mvMat);
			drawPickingFaceShader.SetPMat(pMat);
			model.RenderSelectedFace();
			drawPickingFaceShader.Disable();
		}

		if (drawTexture)
		{
			for (int i = 0; i < model.textures.size(); i++)
			{
				model.currentTexture = i;
				if (model.textures[i]->selectedFace.size() == 0 || model.textures[i]->elemCount.size() == 0)
				{
					continue;
				}

				drawModelShader.DrawTexture(true);
				glBindTexture(GL_TEXTURE_2D, model.textures[i]->textureID);
				model.RenderParameterized();
				glBindTexture(GL_TEXTURE_2D, 0);
			}

			//把currentTexture改回去
			model.currentTexture = TextureBox->SelectedIndex;
		}
		drawModelShader.Disable();

	}
		   void SelectionHandler(unsigned int x, unsigned int y)
		   {
			   GLuint faceID = pickingTexture.ReadTexture(x, windowHeight - y - 1);


			   if (faceID != 0)
			   {
				   currentFaceID = faceID;
			   }

			   if (modeSelection == ADD_FACE)
			   {
				   if (faceID != 0)
				   {
					   model.AddSelectedFace(faceID - 1);
				   }
			   }
			   else if (modeSelection == SMALL)
			   {
				   if (faceID != 0)
				   {
					   model.BrushSelectFace(faceID - 1, 1);
				   }
			   }
			   else if (modeSelection == MEDIUM)
			   {
				   if (faceID != 0)
				   {
					   model.BrushSelectFace(faceID - 1, 2);
				   }
			   }
			   else if (modeSelection == LARGE)
			   {
				   if (faceID != 0)
				   {
					   model.BrushSelectFace(faceID - 1, 3);
				   }
			   }
			   else if (modeSelection == DEL_FACE)
			   {
				   if (faceID != 0)
				   {
					   model.DeleteSelectedFace(faceID - 1);
				   }
			   }
			   else if (modeSelection == DELETE_SMALL)
			   {
				   if (faceID != 0)
				   {
					   model.BrushDeleteFace(faceID - 1, 1);
				   }
			   }
			   else if (modeSelection == DELETE_MEDIUM)
			   {
				   if (faceID != 0)
				   {
					   model.BrushDeleteFace(faceID - 1, 2);
				   }
			   }
			   else if (modeSelection == DELETE_LARGE)
			   {
				   if (faceID != 0)
				   {
					   model.BrushDeleteFace(faceID - 1,3);
				   }
			   }
			   else if (modeSelection == EXPAND_FACE)
			   {
				   if (faceID != 0)
				   {
					   model.ScaleSelectedFace(true);
				   }
			   }
			   else if (modeSelection == DIMINISH_FACE)
			   {
				   if (faceID != 0)
				   {
					   model.ScaleSelectedFace(false);
				   }
			   }
			   

		   }
	private: System::Void hkoglPanelControl1_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
	{
		
		if (e->Button == System::Windows::Forms::MouseButtons::Left)
		{
			meshWindowCam.mousePressEvent(0x0000, e->X, e->Y);
		}
		if (e->Button == System::Windows::Forms::MouseButtons::Middle)
		{
			meshWindowCam.mousePressEvent(0x0001, e->X, e->Y);
		}

		//右鍵
		if (e->Button == System::Windows::Forms::MouseButtons::Right)
		{

			isRightButtonPress = true;
			SelectionHandler(e->X, e->Y);
		}

	}
	private: System::Void hkoglPanelControl1_MouseUp(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
	{
		if (e->Button == System::Windows::Forms::MouseButtons::Left)
		{
			meshWindowCam.mouseReleaseEvent(0x0000, e->X, e->Y);
		}
		if (e->Button == System::Windows::Forms::MouseButtons::Middle)
		{
			meshWindowCam.mouseReleaseEvent(0x0001, e->X, e->Y);
		}

		if (e->Button == System::Windows::Forms::MouseButtons::Right)
		{
			isRightButtonPress = false;
			model.Parameterization();

			//紀錄上一步
			model.recordNumber++;
		
			model.record[model.recordNumber].resize(model.textures.size());
			for (int i = 0; i < model.textures.size(); i++)
			{
				model.record[model.recordNumber][i].selectedFace = model.textures[i]->selectedFace;
				model.record[model.recordNumber][i].elemCount = model.textures[i]->elemCount;
				model.record[model.recordNumber][i].fvIDsPtr = model.textures[i]->fvIDsPtr;
			}

			model.checkNext = false;

		}
	}
	private: System::Void hkoglPanelControl1_MouseMove(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
	{
		meshWindowCam.mouseMoveEvent(e->X, e->Y);

		//點著右鍵選面
		if (isRightButtonPress)
		{
			SelectionHandler(e->X, e->Y);
		}
	}
	private: System::Void hkoglPanelControl1_MouseWheel(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
	{

		if (e->Delta > 0)
		{
			meshWindowCam.wheelEvent(-10);
		}
		else
		{
			meshWindowCam.wheelEvent(10);
		}
	}
	private: System::Void timer1_Tick(System::Object^ sender, System::EventArgs^ e) {

		hkoglPanelControl1->Invalidate();

	}

	private: System::Void Mode_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {

		if (Mode->SelectedItem->ToString() == "Add Face")
		{
			modeSelection = ADD_FACE;
		}
		else if (Mode->SelectedItem->ToString() == "Paint Brush(Small)")
		{
			modeSelection = SMALL;
		}
		else if (Mode->SelectedItem->ToString() == "Paint Brush(Medium)")
		{
			modeSelection = MEDIUM;
		}
		else if (Mode->SelectedItem->ToString() == "Paint Brush(Large)")
		{
			modeSelection = LARGE;
		}
		else if (Mode->SelectedItem->ToString() == "Delete Face")
		{
			modeSelection = DEL_FACE;
		}
		else if (Mode->SelectedItem->ToString() == "Erase Brush(Small)")
		{
			modeSelection = DELETE_SMALL;
		}
		else if (Mode->SelectedItem->ToString() == "Erase Brush(Medium)")
		{
			modeSelection = DELETE_MEDIUM;
		}
		else if (Mode->SelectedItem->ToString() == "Erase Brush(Large)")
		{
			modeSelection = DELETE_LARGE;
		}
		else if (Mode->SelectedItem->ToString() == "Expand Face")
		{
			modeSelection = EXPAND_FACE;
		}
		else if (Mode->SelectedItem->ToString() == "Diminish Face")
		{
			modeSelection = DIMINISH_FACE;
		}

	}
	private: System::Void drawTextureBox_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
		if (drawTextureCheck->Checked == true)
		{
			drawTexture = true;
		}
		else if (drawTextureCheck->Checked == false)
		{
			drawTexture = false;
		}

	}

	private: System::Void ParameterizationButton_Click(System::Object^ sender, System::EventArgs^ e) {
		std::cout << "parameterization" << std::endl;
		
		model.Parameterization();

	}


	private: System::Void TextureBox_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {

		model.currentTexture = TextureBox->SelectedIndex;
	
	}

	private: System::Void SaveButton_Click(System::Object^ sender, System::EventArgs^ e)
	{
		saveFileDialog->Filter = "txt(*.txt)|*txt";
		saveFileDialog->ShowDialog();
	}


private: System::Void LoadButton_Click(System::Object^ sender, System::EventArgs^ e) 
{
	openFileDialog->Filter = "txt(*.txt)|*.txt";
	openFileDialog->Multiselect = false;
	openFileDialog->ShowDialog();

	
}
	private: System::Void loadModelDialog_FileOk(System::Object^ sender, System::ComponentModel::CancelEventArgs^ e) {


		for (int i = 0; i < model.textures.size(); i++)
		{
			model.textures[i]->selectedFace.clear();
			model.textures[i]->elemCount.clear();
			model.textures[i]->fvIDsPtr.clear();
		}

		std::string fileName;

		MarshalString(openFileDialog->FileName, fileName);


		std::ifstream file;
		file.open(fileName, std::ios::in);
		if (file.fail())
		{
			//顯示在LogWindow
			LogWindow->Items->Add("Open File Failed");
			LogWindow->TopIndex = LogWindow->Items->Count - 1;
		}
		else
		{
			int textureAmount, faceAmount;
			unsigned int temp;
			unsigned int* tempfv;
			file >> textureAmount;

			for (int i = 0; i < textureAmount; i++)
			{
				file >> faceAmount;
				for (int j = 0; j < faceAmount; j++)
				{
					file >> temp;
					model.textures[i]->selectedFace.push_back(temp);
				}
				model.currentTexture = i;
				model.Parameterization(uvRotateAngle);
			}
		}

		//顯示在LogWindow
		LogWindow->Items->Add("Open File Successed");
		LogWindow->TopIndex = LogWindow->Items->Count - 1;
	}
		   //把^字串轉成正常字串
	   void MarshalString(System::String^ s, std::string& os)
	   {
		   using namespace System::Runtime::InteropServices;
		   const char* chars = (const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
		   os = chars;
		   Marshal::FreeHGlobal(System::IntPtr((void*)chars));
	   }

	private: System::Void saveFileDialog_FileOk(System::Object^ sender, System::ComponentModel::CancelEventArgs^ e) {
		std::string fileName;
		MarshalString(saveFileDialog->FileName, fileName);


		//std::cout << "Enter file name: ";
		//std::cin >> fileName;
		fileName += ".txt";

		std::ofstream file;
		file.open(fileName, std::ios::out);

		file << model.textures.size() << std::endl;
		for (int i = 0; i < model.textures.size(); i++)
		{
			file << model.textures[i]->selectedFace.size() << std::endl;
			for (int j = 0; j < model.textures[i]->selectedFace.size(); j++)
			{
				file << model.textures[i]->selectedFace[j] << std::endl;
			}

		}
		//顯示在LogWindow
		LogWindow->Items->Add("Save File Successed");
		LogWindow->TopIndex = LogWindow->Items->Count - 1;
		file.close();
	}
	private: System::Void RecoveryButton_Click(System::Object^ sender, System::EventArgs^ e) {
		
		if (model.recordNumber > 0)
		{

			model.recordNumber--;
			model.record[model.recordNumber].resize(model.textures.size());

			for (int i = 0; i < model.textures.size(); i++)
			{
				model.textures[i]->selectedFace = model.record[model.recordNumber][i].selectedFace;
				model.textures[i]->elemCount = model.record[model.recordNumber][i].elemCount;
				model.textures[i]->fvIDsPtr = model.record[model.recordNumber][i].fvIDsPtr;
			
			}

			for (int i = 0; i < model.textures.size(); i++)
			{
				std::cout << model.textures[i]->selectedFace.size() << std::endl;
			}
			model.Parameterization();
			model.checkNext = true;

			//顯示在LogWindow
			LogWindow->Items->Add("Undo");
			LogWindow->TopIndex = LogWindow->Items->Count - 1;
		}
		else
		{
			puts("沒有上一步");

			//顯示在LogWindow
			LogWindow->Items->Add("沒有上一步");
			LogWindow->TopIndex = LogWindow->Items->Count - 1;
		}

	}
	private: System::Void NextButton_Click(System::Object^ sender, System::EventArgs^ e) {
		
		if (model.checkNext == true)
		{
			if (model.recordNumber < model.record.size() - 1)
			{
				model.recordNumber++;
				for (int i = 0; i < model.textures.size(); i++)
				{
					model.textures[i]->selectedFace = model.record[model.recordNumber][i].selectedFace;
					model.textures[i]->elemCount = model.record[model.recordNumber][i].elemCount;
					model.textures[i]->fvIDsPtr = model.record[model.recordNumber][i].fvIDsPtr;

				}

				for (int i = 0; i < model.textures.size(); i++)
				{
					std::cout << model.textures[i]->selectedFace.size() << std::endl;
				}

				model.Parameterization();
				//顯示在LogWindow
				LogWindow->Items->Add("Redo");
				LogWindow->TopIndex = LogWindow->Items->Count - 1;
			}
			else
			{

				//顯示在LogWindow
				LogWindow->Items->Add("沒有下一步");
				LogWindow->TopIndex = LogWindow->Items->Count - 1;
			}
		}
		else
		{

			//顯示在LogWindow
			LogWindow->Items->Add("下一步已更改");
			LogWindow->TopIndex = LogWindow->Items->Count - 1;
		}

	}


private: System::Void AddTextureDialog_FileOk(System::Object^ sender, System::ComponentModel::CancelEventArgs^ e) {
	std::string fileName;

	MarshalString(AddTextureDialog->FileName, fileName);

	if (TextureBox->Items->Contains(AddTextureDialog->FileName))
	{
		//顯示在LogWindow
		LogWindow->Items->Add("有同名檔案");
		LogWindow->TopIndex = LogWindow->Items->Count - 1;
		return;
	}

	std::string path = fileName;
	paths.push_back(path);

	model.AddTexture(paths[paths.size() - 1]);

	TextureBox->Items->Add(AddTextureDialog->FileName);
	
	//顯示在LogWindow
	LogWindow->Items->Add("Add Texture");
	LogWindow->TopIndex = LogWindow->Items->Count - 1;
}
	 
private: System::Void addToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
	AddTextureDialog->Multiselect = false;
	AddTextureDialog->ShowDialog();
}
private: System::Void deleteToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
	model.DeleteTexture(model.currentTexture);
	//刪掉GUI裡面的選項
	int removeItemIndex = TextureBox->SelectedIndex;
	TextureBox->SelectedIndex++;
	TextureBox->Items->RemoveAt(removeItemIndex);
	//顯示在LogWindow
	LogWindow->Items->Add("Delete Texture");
	LogWindow->TopIndex = LogWindow->Items->Count - 1;
}
private: System::Void openToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
	openFileDialog->Filter = "txt(*.txt)|*.txt";
	openFileDialog->Multiselect = false;
	openFileDialog->ShowDialog();
}
private: System::Void saveToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
	saveFileDialog->Filter = "txt(*.txt)|*txt";
	saveFileDialog->ShowDialog();
}

};
}