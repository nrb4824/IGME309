#include "AppClass.h"
void Application::InitVariables(void)
{
	//Change this to your name and email
	m_sProgrammer = "Your Name Here";

	//Does not need changes
	m_fRadius = 1.0f;
	m_fHeight = 5.0f;

	m_pPencil = new MyMesh();
	m_pPencil->GeneratePencil( 12, m_fRadius, m_fHeight);
}
#pragma region DOES NOT NEED CHANGES
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();

	//Get some variable to position and rotate the other shapes out of the origin
	static uint uClock = m_pSystem->GenClock();
	float fDelta = m_pSystem->GetDeltaTime(uClock);
	static float fCumulative = 0.0f;
	float fFPS = 60.0f;
#pragma endregion DOES NOT NEED CHANGES
	if (m_qArcBall == glm::quat())
	{
		/* Your matrix multiplication to make the spiral goes here remember to multiply the
		rotation by fFPS or it will go really slow, below there is an example on an unnecessary scale*/

		m_m4Model = glm::scale(vector3(1.0f));
	}
#pragma region DOES NOT NEED CHANGES
	else
	{
		//Does not need changes
		m_m4Model = ToMatrix4(m_qArcBall);
	}
	fCumulative += fDelta;
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	//Render the model
	m_pPencil->Render(
			m_pCameraMngr->GetProjectionMatrix(), 
			m_pCameraMngr->GetViewMatrix(), 
			m_m4Model);
	
	// draw a skybox
	m_pModelMngr->AddSkyboxToRenderList();

	// add grid to scene
	if(m_bGrid)
		m_pModelMngr->AddGridToRenderList();
	
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
	//Release the mesh list
	SafeDelete(m_pPencil);
	
	//release GUI
	ShutdownGUI();
}
#pragma endregion DOES NOT NEED CHANGES