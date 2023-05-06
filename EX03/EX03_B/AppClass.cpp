#include "AppClass.h"
void Application::InitVariables(void)
{
	////Change this to your name and email
	m_sProgrammer = "Alberto Bobadilla - labigm@rit.edu";
	vector3 v3Position(0.0f, 0.0f, 250.0f);
	vector3 v3Target(0.0f, 0.0f, 0.0f);
	vector3 v3Upward = AXIS_Y;
	m_pCameraMngr->SetCameraMode(BTXs::CAM_ORTHO_Z);
	m_pCameraMngr->SetPositionTargetAndUpward(v3Position, v3Target, v3Upward);

	m_pEntityMngr = EntityManager::GetInstance(); //Initialize the entity manager
	m_pEntityMngr->AddEntity("Minecraft\\Cube.obj", "Cube");
	m_pEntityMngr->UsePhysicsSolver();

#ifdef DEBUG
	m_uObjects = 500;
#else
	m_uObjects = 1500;
#endif // DEBUG

	for (int i = 0; i < m_uObjects; i++)
	{
		m_pEntityMngr->AddEntity("Minecraft\\Cube.obj", "Cube_" + std::to_string(i)); 
		vector2 temp = glm::circularRand(12.0f);
		vector3 v3Position = vector3(0.0f, 12.0f, 0.0f) + vector3(temp.x, temp.y, 0.0f);
		matrix4 m4Position = glm::translate(v3Position);
		m_pEntityMngr->SetModelMatrix(m4Position * glm::scale(vector3(1.0f)) * glm::rotate(IDENTITY_M4, glm::radians(45.0f), AXIS_Y));
		m_pEntityMngr->UsePhysicsSolver(true);
		m_pEntityMngr->AddDimension(-1,i);//Sets each cube in a different space, the grid will override this behavior 
	}
	m_uGridWidth = 4;
	m_uGridHeight = 5;
	m_pRoot = new Space(m_uGridWidth, m_uGridHeight);
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();

	//This is a routine to start the grid every quarter of a second
	static uint nClock = m_pSystem->GenClock();
	float fTimer = m_pSystem->GetTimeSinceStart(nClock);
	if (fTimer > 0.01)
	{
		SafeDelete(m_pRoot);
		m_pRoot = new Space(m_uGridWidth, m_uGridHeight);
		m_pSystem->ResetClock(nClock);
	}

	//Update Entity Manager
	m_pEntityMngr->Update();

	//Add objects to render list
	m_pEntityMngr->AddEntityToRenderList(-1, true);
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	//display grid
	m_pRoot->Display();
	
	matrix4 m4Model = ToMatrix4(m_qArcBall);//model matrix

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
	SafeDelete(m_pRoot);
	//release GUI
	ShutdownGUI();
}