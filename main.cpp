#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <shader/shader_s.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// 설정값
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

<<<<<<< HEAD
int main()
{
    // glfw 초기화 및 설정
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // OpenGL 버전 설정
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // OpenGL 버전 설정
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // OpenGL 코어 프로파일 사용
=======
    // Vertex Shader 소스코드. GLSL로 작성한다.
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
>>>>>>> 84a76cae3c7b6572df812676dbae0566f1b6f3e6

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Mac OS에서는 호환성 모드 설정
#endif

    // glfw 윈도우 생성
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "GLFW 윈도우 생성 실패" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); // 현재 윈도우를 OpenGL 컨텍스트로 설정
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // 윈도우 크기 변경 시 콜백 함수 설정

    // glad: 모든 OpenGL 함수 포인터 로드
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "GLAD 초기화 실패" << std::endl;
        return -1;
    }

    // 셰이더 프로그램 빌드 및 컴파일
    // ------------------------------------
    Shader ourShader("3.3.shader.vs", "3.3.shader.fs"); // 셰이더 파일 경로

    // 정점 데이터 설정 (VBO, VAO 생성 및 정점 속성 설정)
    // ------------------------------------------------------------------
    float vertices[] = {
        // 위치               // 색상
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // 오른쪽 아래
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // 왼쪽 아래
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // 위쪽
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO); // VAO 생성
    glGenBuffers(1, &VBO); // VBO 생성
    // 먼저 VAO를 바인딩하고, 그 다음 VBO를 바인딩하여 데이터를 설정한 후 정점 속성을 설정
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 위치 속성
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // 색상 속성
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // VAO와 VBO의 바인딩 해제 (필수는 아니지만 일반적으로 VAO를 바인딩 해제하는 것이 좋습니다)
    // glBindVertexArray(0);

    // 렌더링 루프
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // 입력 처리
        // -----
        processInput(window);

        // 화면을 지우고 렌더링
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 배경 색상 설정
        glClear(GL_COLOR_BUFFER_BIT); // 화면 클리어

        // 삼각형 렌더링
        ourShader.use(); // 셰이더 프로그램 사용
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3); // 삼각형 그리기

        // glfw: 버퍼를 교환하고 IO 이벤트 처리 (키보드, 마우스 등)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window); // 버퍼 교환
        glfwPollEvents(); // 이벤트 처리
    }

    // 리소스 해제
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO); // VAO 삭제
    glDeleteBuffers(1, &VBO); // VBO 삭제

    // glfw 종료
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// 입력 처리 함수: 키보드 입력을 확인하고 종료 시 ESC 키를 누르면 창을 닫음
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) // ESC 키가 눌렸을 때
        glfwSetWindowShouldClose(window, true); // 윈도우 닫기
}

<<<<<<< HEAD
// glfw: 윈도우 크기 변경 시 호출되는 콜백 함수
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // 새로운 윈도우 크기에 맞게 뷰포트를 설정
    glViewport(0, 0, width, height);
}
=======
    // 입력 처리 함수: ESC 키를 누르면 창 닫기 플래그를 설정
    // ----------------------------------------------------------------------------
void processInput(GLFWwindow *window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE)== GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
    // glfwGetKey: 특정 키의 현재 상태를 반환함.
>>>>>>> 84a76cae3c7b6572df812676dbae0566f1b6f3e6
