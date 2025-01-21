#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

    // 창 크기 변경 콜백 함수 및 입력 처리 함수 선언
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);


    // Vertex Shader 소스코드. GLSL로 작성한다.
const char *vertexShaderSource = "#version 33- core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

    // 프래그먼트 셰이더 소스 코드
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n" // 출력될 색상 변수
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n" // 색상 값 설정
    "}\n\0";




int main(){
    glfwInit(); // glfwinit은 GLFW 라이브러리를 초기화한다. GLFW 함수를 쓰기 전에 반드시 실행.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3); // OpenGL 주요 버전 설정 (3.x)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3); // OpenGL 부 버전 설정 (3.3)
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE); // OpenGL Core Profile 사용

    // GLFW 창 생성
    // --------------------
    GLFWwindow* window = glfwCreateWindow(800,600,"learnopengl",NULL,NULL); 
    // glfwCreateWindow는 윈도우를 만들고 그것과 관련된 Context를 만든다.  Context의 옵션은 glfwWindowHint로 정해진다.
    if(window == NULL){
        std::cout << "윈도우 생성 실패!" << std::endl;
        glfwTerminate();
        return -1; 
        // 창 생성 실패시 오류 메시지 출력하고 리소스 해제하고 프로그램 종료.
    }
    glfwMakeContextCurrent(window); // OpenGL 명령을 해당 윈도우(window)에 적용하겠다. OpenGL 함수를 쓰려면 GLAD 초기화를 먼저 한다.
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // 창 크기 변경 시 뷰포트 조정하는 콜백 함수.

    // GLAD 초기화: OpenGL 함수 로드
    // ---------------------------------------
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "GLAD 초기화 실패!" << std::endl;
        return -1;
    }
    // gladLoadGLLoader를 호출하여 OpenGL 함수 포인터를 로드한다. 
    // glfwGetProcAddress는 OpenGL 함수의 주소를 가져오는 역할을 한다.

    //셰이더 프로그램 생성 및 컴파일
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER); // 버텍스 셰이더 생성.
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // 셰이더 소스코드 첨부.
    glCompileShader(vertexShader); // 셰이더 컴파일.

    //컴파일이 성공했는지 확인하려면 glCompileShader 호출 후, 오류가 있는지 체크해야 합니다. 
    int success; //컴파일 성공 여부를 나타내는 정수 success
    char infoLog[512]; // 오류메시지를 저장할 변수 infoLog
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success); //컴파일 성공여부 확인 함수.

    //만약 컴파일이 실패했다면, glGetShaderInfoLog로 오류 메시지를 받아와 출력한다.
    if(!success){
        glGetShaderInfoLog(vertexShader,512,NULL,infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); //fragment셰이더 생성.
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL); //셰이더 소스코드 첨부.
    glCompileShader(fragmentShader); // 셰이더 컴파일.
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(fragmentShader,512,NULL,infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //두 셰이더가 컴파일되면, 두 셰이더를 하나의 셰이더 프로그램에 연결해야 한다. 이렇게 하면 OpenGL에서 렌더링에 사용할 수 있다.
    unsigned int shaderProgram = glCreateProgram(); //셰이더 프로그램 생성.
    //컴파일된 셰이더들을 이 프로그램에 연결해야 합니다. 이를 위해 glAttachShader로 셰이더들을 프로그램에 첨부하고, glLinkProgram으로 셰이더들을 연결합니다:
    glAttachShader(shaderProgram, vertexShader); //버텍스 셰이더 첨부
    glAttachShader(shaderProgram, fragmentShader); //프래그먼트 셰이터 첨부
    glLinkProgram(shaderProgram); //셰이더링크

    if(!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    } //셰이더 생성과정과 비슷하게 생성후 링크가 제대로 되었는지 확인하는 과정.

    //셰이더 프로그램이 제대로 링크된 후에는 더 이상 개별 셰이더 객체가 필요하지 않으므로, glDeleteShader를 사용하여 삭제해주면 된다.
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //버텍스 셰이더는 우리가 원하는 입력을 버텍스 속성 형식으로 지정할 수 있게 해줍니다. 
    //이는 큰 유연성을 제공하지만, 입력 데이터가 셰이더의 어느 버텍스 속성으로 연결될지 수동으로 지정해야 한다는 의미도 있습니다. 
    //즉, 우리가 OpenGL에게 어떻게 버텍스 데이터를 해석할지 알려줘야 한다는 것입니다.
    //버텍스 속성 연결 이라고 부르는데, 그 전에 VBO,VAO를 먼저 생성하는것이 낫다.

     // 정점 데이터와 버퍼 및 VAO 설정
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // 왼쪽 아래
         0.5f, -0.5f, 0.0f, // 오른쪽 아래
         0.0f,  0.5f, 0.0f  // 위쪽
    }; 

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO); //VAO생성
    glGenBuffers(1,&VBO); //VBO생성
    
    glBindVertexArray(VAO);
    //OpenGL에서 VBO를 생성하고(glGenBuffers), 활성화합니다(glBindBuffer).
    glBindBuffer(GL_ARRAY_BUFFER, VBO); 
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices), vertices, GL_STATIC_DRAW);
    //이 단계에서 VBO는 VAO와 자동으로 연결된다. VAO는 “GL_ARRAY_BUFFER 타입으로 바인딩된 VBO”를 기록.

    glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE, 3 * sizeof(float) , (void*)0); // glVertexAttribPointer 함수를 사용하여 OpenGL에게 버텍스 데이터를 어떻게 해석할지 지정할 수 있습니다. 
    glEnableVertexAttribArray(0); //이제 OpenGL에게 버텍스 데이터를 어떻게 해석해야 할지 알려줬으므로, glEnableVertexAttribArray 함수를 사용하여 해당 속성을 활성화한다. 기본적으로 버텍스 속성은 비활성화되어 있기 때문에 이를 명시적으로 활성화해 줘야 한다.
    //정점 데이터를 셰이더와 연결하는 설정을 VAO에 기록한 과정들이다.VAO를 바인딩 해제하여 기록 작업을 종료하자.
    glBindBuffer(GL_ARRAY_BUFFER,0); //VBO 언바인딩.
    glBindVertexArray(0); //VAO 언바인딩으로 기록 종료.

    // 렌더링 루프
    // ----------- 한번만 그리고 프로그램을 종료하는 것이 아닌, 사용자가 종료하라고 할 때까지 프로그램이 계속해서 이미지를 그리기를 원하므로 while문을 사용한다. 렌더 루프라고 한다.
    while(!glfwWindowShouldClose(window)){ // while문의 시작 부분에서 지속적으로 GLFW가 종료받기를 지시받았는지 확인한다.
        
        processInput(window); 
        // ESC 입력하면 while문이 True가 되면서 탈출. 즉, 창 닫기 요청(ESC 키 입력 등) 시 glfwSetWindowShouldClose(window, true)가 호출되면 glfwWindowShouldClose(window)가 true를 반환하고 루프가 종료된다.

        glClearColor(0.2f,0.3f,0.3f,1.0f); // 배경색을 설정.
        glClear(GL_COLOR_BUFFER_BIT); // 화면을 초기화. 그중에서도 컬러 버퍼를 지우는 것이다.

        glUseProgram(shaderProgram);
        //shaderProgram이라는 셰이더 프로그램을 활성화하여 이후의 렌더링에서 이 프로그램을 사용하도록 설정함.
        glBindVertexArray(VAO);
        //VAO(Vertex Array Object)를 바인딩하여, 이 VAO에 설정된 모든 버텍스 속성 (예: VBO 연결 정보 등)을 사용하도록 지정.
        glDrawArrays(GL_TRIANGLES, 0, 3);
        //glDrawArrays(GL_TRIANGLES, 0, 3);는 배열의 0번 인덱스부터 3개의 정점을 사용하여 삼각형을 그린다는 의미이다.
        
        glfwSwapBuffers(window); // 더블 버퍼링을 수행. 후면 버퍼에 새 프레임을 렌더링한 후, glfwSwapBuffers를 호출해 전면 버퍼와 교체하며 깜빡임 방지.
        glfwPollEvents();
        // 프로그램에서 발생하는 다양한 이벤트를 수집하고 처리하는 작업. 여기서 이벤트는 키 입력뿐만 아니라 창 크기 조정, 마우스 움직임, 창 닫기 신호 등 프로그램과 관련된 모든 상호작용을 포함한다.
        // 예를 들어 ESC가 눌리면 glfwPollEvents();가 이벤트를 감지하고, processInput()에서 ESC 키 입력을 확인하고, 창을 닫도록 설정한다. 다음 루프 때 True가 되면서 종료된다.
    }

    //렌더루프가 끝나면 사실상 모든 자원들을 해제하는 과정이 필요하다. GPU메모리를 최적화하기 위해서 VAO,VBO등을 해제하고, GLFW관련해서는 glfwTerminate()를 사용해준다.
    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1,&VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate(); // 마지막에는 모든 자원들을 해제한다.
    return 0;
}

    // 창 크기 변경 콜백 함수: OpenGL 뷰포트를 새로운 창 크기에 맞게 조정
    // ---------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0,0,width,height); // OpenGL에게 '렌더링할 부분'의 윈도우의 크기를 말해주는 함수. 매개변수 범위는 (-1 ~ +1)이다.
}

    // 입력 처리 함수: ESC 키를 누르면 창 닫기 플래그를 설정
    // ----------------------------------------------------------------------------
void processInput(GLFWwindow *window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE)== GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
    // glfwGetKey: 특정 키의 현재 상태를 반환함.