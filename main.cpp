#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height); //
void processInput(GLFWwindow *window);

int main(){
    glfwInit(); //glfwinit은 GLFW 라이브러리를 초기화한다. GLFW 함수를 쓰기전에 반드시 실행.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3); //OpenGL 주요 버전 설정 (3.x)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3); // OpenGL 부 버전 설정 (3.3)
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE); // OpenGL Core Profile 사용


    // GLFW 창 생성
    // --------------------
    GLFWwindow* window = glfwCreateWindow(800,600,"learnopengl",NULL,NULL); 
    // glfwCreateWindow는 윈도우를 만들고 그것과 관련된 Context를 만든다. Context의 옵션은 glfwWindowHint로 정해진다.
    if(window == NULL){
        std::cout << "윈도우 생성 실패!" << std::endl;
        glfwTerminate();
        return -1; 
        //창 생성 실패시 오류메시지 출력하고 리소스 해제하고 프로그램 종료.
    }
    glfwMakeContextCurrent(window); // OpenGL 명령을 해당 윈도우(window)에 적용하겠다. OpenGL함수를 쓰려면 GLAD초기화를 먼저한다.
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // 창 크기 변경 시 뷰포트조정하는 콜백 함수.

    // GLAD 초기화: OpenGL 함수 로드
    // ---------------------------------------
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
    {
        std::cout << "GLAD 초기화 실패!" << std::endl;
        return -1;
    }
    //gladLoadGLLoader를 호출하여 OpenGL 함수 포인터를 로드한다. 
    //gladLoadGLLoader 함수의 매개변수는 OpenGL 함수의 주소를 가져오는 함수 포인터를 전달해야 한다.
    //glfwGetProcAddress는 OpenGL 함수의 주소를 가져오는 역할을 한다.

    // 렌더링 루프
    // ----------- 한번만 그리고 프로그램을 종료하는것이 아닌, 사용자가 종료하라고 할 때까지 프로그램이 계속해서 이미지를 그리기를 원하므로 while문을 사용한다. 렌더루프라고 한다.
    while(!glfwWindowShouldClose(window)){ // while문의 시작부분에서 지속적으로 GLFW가 종료받기를 지시받았는지 확인한다.
        
        processInput(window); // ESC입력하면 while문이 True가 되면서 탈출. 즉 ,창 닫기 요청(ESC 키 입력 등) 시 glfwSetWindowShouldClose(window, true)가 호출되면 glfwWindowShouldClose(window)가 true를 반환하고 루프가 종료된다.

        glClearColor(0.2f,0.3f,0.3f,1.0f); //배경색을 설정.
        glClear(GL_COLOR_BUFFER_BIT); //화면을 초기화. 그중에서도 컬러 버퍼를 지우는 것이다.
        
        glfwSwapBuffers(window); //더블 버퍼링을 수행. 후면 버퍼에 새 프레임을 렌더링한 후, glfwSwapBuffers를 호출해 전면 버퍼와 교체하며 깜빡임 방지.
        glfwPollEvents();
        //프로그램에서 발생하는 다양한 이벤트를 수집하고 처리하는 작업. 여기서 이벤트는 키 입력뿐만 아니라 창 크기 조정, 마우스 움직임, 창 닫기 신호 등 프로그램과 관련된 모든 상호작용을 포함한다.
        //예를들어 ESC가 눌리면 glfwPollEvents(); 가 이벤트를 감지하고, processInput()에서 ESC 키 입력을 확인하고, 창을 닫도록 설정한다. 다음 루프 때 True가 되면서 종료된다.
    }

    glfwTerminate(); //마지막에는 모든 자원들을 해제한다.
    return 0;
}



// 창 크기 변경 콜백 함수: OpenGL 뷰포트를 새로운 창 크기에 맞게 조정
// ---------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0,0,width,height); //opengl에게 '렌더링할 부분'의 윈도우의 크기를 말해주는 함수. 매개변수 범위는 (-1 ~ +1)이다.
}


// 입력 처리 함수: ESC 키를 누르면 창 닫기 플래그를 설정
// ----------------------------------------------------------------------------
void processInput(GLFWwindow * window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE)== GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
//glfwGetKey: 특정 키의 현재 상태를 반환함.
