#include "AppClass.h"
void Application::InitVariables(void)
{
	////Change this to your name and email
	m_sProgrammer = "Freya Emerson - fse5498@rit.edu";
	vector3 v3Position(0.0f, 0.0f, 30.0f);
	vector3 v3Target = ZERO_V3;
	vector3 v3Upward = AXIS_Y;
	m_pCameraMngr->SetPositionTargetAndUpward(v3Position, v3Target, v3Upward);
	
	//creating meshes
	m_uMeshCount = 46;
	for (uint i = 0; i < m_uMeshCount; i++)
	{
		MyMesh* pMesh = new MyMesh();
		m_pMeshList.push_back(pMesh);
		m_pMeshList[i]->GenerateCube(1.0f, C_BLACK);
	}
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	matrix4 m4View = m_pCameraMngr->GetViewMatrix();
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();

	//Make a timer
	static float fTimer = 0;
	static uint uClock = m_pSystem->GenClock();
	fTimer += static_cast<float>(m_pSystem->GetDeltaTime(uClock));
	
	//Drawing each mesh unit
	uint i = 45;
	static float fHorizontal = -10.0f;
	static vector3 v3Position(fHorizontal, 0.0f, 0.0f);

	v3Position.x = fHorizontal + fTimer;
	v3Position.y = glm::cos(glm::radians(fTimer * 60.0f));
	matrix4 m4Position = glm::translate(vector3(-5.0f, -3.0f, -15.0f)) * glm::translate(v3Position);

	//top row
	m_pMeshList[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(2, 7.0f, 0.0f))); i--;
	m_pMeshList[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(8, 7.0f, 0.0f))); i--;

	//2nd row from top
	m_pMeshList[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(3, 6.0f, 0.0f))); i--;
	m_pMeshList[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(7, 6.0f, 0.0f))); i--;

	//3rd row from top
	m_pMeshList[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(2, 5.0f, 0.0f))); i--;
	m_pMeshList[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(3, 5.0f, 0.0f))); i--;
	m_pMeshList[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(4, 5.0f, 0.0f))); i--;
	m_pMeshList[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(5, 5.0f, 0.0f))); i--;
	m_pMeshList[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(6, 5.0f, 0.0f))); i--;
	m_pMeshList[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(7, 5.0f, 0.0f))); i--;
	m_pMeshList[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(8, 5.0f, 0.0f))); i--;

	//4th row from top
	m_pMeshList[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(1, 4.0f, 0.0f))); i--;
	m_pMeshList[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(2, 4.0f, 0.0f))); i--;
	m_pMeshList[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(4, 4.0f, 0.0f))); i--;
	m_pMeshList[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(5, 4.0f, 0.0f))); i--;
	m_pMeshList[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(6, 4.0f, 0.0f))); i--;
	m_pMeshList[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(8, 4.0f, 0.0f))); i--;
	m_pMeshList[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(9, 4.0f, 0.0f))); i--;

	//5th row from top
	m_pMeshList[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(0, 3.0f, 0.0f))); i--;
	m_pMeshList[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(1, 3.0f, 0.0f))); i--;
	m_pMeshList[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(2, 3.0f, 0.0f))); i--;
	m_pMeshList[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(3, 3.0f, 0.0f))); i--;
	m_pMeshList[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(4, 3.0f, 0.0f))); i--;
	m_pMeshList[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(5, 3.0f, 0.0f))); i--;
	m_pMeshList[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(6, 3.0f, 0.0f))); i--;
	m_pMeshList[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(7, 3.0f, 0.0f))); i--;
	m_pMeshList[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(8, 3.0f, 0.0f))); i--;
	m_pMeshList[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(9, 3.0f, 0.0f))); i--;
	m_pMeshList[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(10, 3.0f, 0.0f))); i--;

	//6th row from top
	m_pMeshList[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(0, 2.0f, 0.0f))); i--;
	m_pMeshList[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(2, 2.0f, 0.0f))); i--;
	m_pMeshList[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(3, 2.0f, 0.0f))); i--;
	m_pMeshList[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(4, 2.0f, 0.0f))); i--;
	m_pMeshList[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(5, 2.0f, 0.0f))); i--;
	m_pMeshList[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(6, 2.0f, 0.0f))); i--;
	m_pMeshList[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(7, 2.0f, 0.0f))); i--;
	m_pMeshList[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(8, 2.0f, 0.0f))); i--;
	m_pMeshList[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(10, 2.0f, 0.0f))); i--;

	//7th row from top
	m_pMeshList[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(0, 1.0f, 0.0f))); i--;
	m_pMeshList[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(2, 1.0f, 0.0f))); i--;
	m_pMeshList[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(8, 1.0f, 0.0f))); i--;
	m_pMeshList[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(10, 1.0f, 0.0f))); i--;

	//bottom row
	m_pMeshList[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(3, 0.0f, 0.0f))); i--;
	m_pMeshList[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(4, 0.0f, 0.0f))); i--;
	m_pMeshList[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(6, 0.0f, 0.0f))); i--;
	m_pMeshList[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(7, 0.0f, 0.0f))); i--;

	// draw a skybox
	m_pModelMngr->AddSkyboxToRenderList();

	//render list call
	m_uRenderCallCount = m_pModelMngr->Render();

	//clear the render list
	m_pModelMngr->ClearRenderList();

	//draw gui
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	//releasing the meshes
	std::vector<MyMesh*> m_pMeshList;
	int uSize = m_pMeshList.size();
	for (int i = 0; i < uSize; i++)
	{
		if (m_pMeshList[i] != nullptr)
		{
			delete m_pMeshList[i];
			m_pMeshList[i] = nullptr;
		}
	}
	//release GUI
	ShutdownGUI();
}