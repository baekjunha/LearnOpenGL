# hello window

## 왜 glad를 먼저 #include하는가?

핵심 이유는 컴파일 단계에서 OpenGL 함수 포인터 정의 때문입니다.

GLAD를 GLFW보다 먼저 포함하지 않으면 GLAD가 OpenGL 함수 포인터를 정의하지 못해서 컴파일 에러가 발생할 가능성이 있습니다. 여기서 중요한 포인트는 OpenGL 헤더와 GLFW 헤더가 서로 함수 정의 충돌을 피하기 위해 특별한 처리를 필요로 한다는 점입니다.

더 자세히 살펴보면:
1.GLFW도 내부적으로 OpenGL 헤더를 포함한다.
•GLFW/glfw3.h 파일은 OpenGL과의 연동을 위해 <GL/gl.h> 또는 플랫폼에 따라 적절한 OpenGL 헤더 파일을 포함합니다.
•문제는, 만약 GLAD를 먼저 포함하지 않으면 GLFW가 기본 OpenGL 헤더를 불러오게 되는데, 이 헤더는 함수 포인터를 선언하지 않습니다.
•GLAD는 OpenGL 함수 포인터를 제공하므로, 이를 GLFW보다 먼저 포함해야 올바른 함수 포인터가 정의됩니다.
2.컴파일 타임에 헤더 파일 순서가 중요하다.
•GLAD는 #define GLAD_GL 같은 매크로를 통해 OpenGL 함수 포인터를 정의하고 초기화 코드를 제공하는데, 만약 GLFW가 먼저 포함되면 GLAD가 이러한 정의를 덮어쓰지 못할 수 있습니다.
•결과적으로 OpenGL 관련 함수 (예: glClear, glViewport)를 사용할 때, 컴파일러는 이를 알 수 없어서 undefined reference 에러가 발생합니다.
3.GLAD는 GLFW와 독립적이다.
•GLAD는 OpenGL 함수에만 초점을 맞춘 라이브러리이고, GLFW는 창 생성과 이벤트 처리 같은 기능에 더 집중합니다.
•GLAD를 먼저 포함하지 않으면, GLFW는 OpenGL 함수에 대한 초기화를 전혀 처리하지 않으므로 glClear나 glViewport 같은 OpenGL 함수 호출이 제대로 작동하지 않을 수 있습니다.

정리하면:
•코드 실행 순서와 관계없이 GLAD는 컴파일러가 OpenGL 함수 포인터를 정확히 정의할 수 있도록 헤더 파일 순서를 지켜야 합니다.
•따라서, GLAD는 항상 GLFW보다 먼저 포함해야 하고, 이는 컴파일 타임 문제를 예방하기 위한 규칙입니다.

혹시 컴파일 과정이나 함수 정의 충돌 문제를 더 깊게 알고 싶다면, 추가로 설명해줄게! 😊

## Context란 정확히 뭔지 모르겠다.

Context는 OpenGL의 상태와 동작을 관리하는 환경을 말합니다.
OpenGL은 상태 기반 API로, 모든 명령은 현재 활성화된 OpenGL Context에서 실행됩니다.

Context에는 다음과 같은 정보가 포함됩니다:
•렌더링 상태: 뷰포트, 컬러 버퍼, 깊이 버퍼, 스텐실 버퍼 등.
•연결된 리소스: 셰이더 프로그램, 텍스처, VAO(VAO: Vertex Array Object) 등.
•렌더링 옵션: OpenGL의 렌더링 동작을 제어하는 다양한 설정.

glfwCreateWindow는 창과 관련된 OpenGL Context를 생성합니다. 그런 다음 glfwMakeContextCurrent(window)를 호출하면 OpenGL 명령이 해당 Context에서 실행되도록 설정됩니다. 이렇게 하지 않으면 OpenGL 명령을 호출해도 동작하지 않습니다.

비유하자면, Context는 “OpenGL의 작업 공간”이고, glfwMakeContextCurrent는 “우리가 지금 사용하는 작업 공간”을 설정하는 것입니다.

## 20-21번 라인은 왜 저때 쓰는가?
   
```cpp
glfwMakeContextCurrent(window);
glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
```

1.glfwMakeContextCurrent(window)
•OpenGL 명령을 실행하려면 반드시 활성화된 Context가 필요합니다.
•glfwCreateWindow는 창과 Context를 생성하지만, 이를 사용 가능하도록 활성화하려면 glfwMakeContextCurrent(window)를 호출해야 합니다.
•OpenGL의 초기화 작업(GLAD 로드, 셰이더 프로그램 생성 등)은 Context가 활성화된 후에만 가능합니다. 따라서 이 줄을 먼저 호출해야 합니다.  

2.glfwSetFramebufferSizeCallback(window, framebuffer_size_callback)
•이 함수는 창 크기가 변경될 때 호출될 콜백 함수를 등록합니다.
•창 크기 변경 이벤트는 GLFW에서 관리되므로, window 객체와 연결된 Context가 활성화된 상태에서 콜백 함수를 설정해야 정상적으로 동작합니다.
•Context가 활성화된 후에 콜백을 등록해야 OpenGL 명령(예: glViewport)을 호출할 때 문제가 발생하지 않습니다.

## 렌더 루프에서 processInput과 glfwPollEvents 함수의 관계성

processInput(window)
•OpenGL 창과 관련된 특정 사용자 입력 이벤트를 처리하는 함수입니다.
•예제에서는 glfwGetKey를 사용해 ESC 키가 눌렸는지 확인한 뒤, 창 닫기 플래그(glfwSetWindowShouldClose)를 설정하고 있습니다.
•역할: 특정 키 입력에 따른 로직을 처리.

glfwPollEvents()
•GLFW가 관리하는 모든 이벤트를 수집하고 처리합니다.
•창 크기 변경, 마우스 움직임, 키보드 입력, 창 닫기 신호 등.
•GLFW는 이벤트 큐를 사용해 이벤트를 저장하므로, glfwPollEvents()는 이 큐를 처리하고 관련 콜백 함수(예: 창 크기 변경 시 framebuffer_size_callback)를 실행합니다.

두 함수의 관계
•processInput은 키보드 입력에 대한 특정 로직을 처리하는 사용자 정의 함수이고, glfwPollEvents는 GLFW가 이벤트 큐를 관리하며 기본적으로 이벤트 처리를 수행합니다.
•glfwPollEvents()를 호출하지 않으면, GLFW는 이벤트를 감지하지 못하고 입력 처리도 수행되지 않기 때문에 processInput에서 ESC 키 입력을 확인할 수 없습니다.

요약:
•glfwPollEvents()는 이벤트를 큐에서 꺼내고 이를 GLFW 내부적으로 처리.
•processInput은 특정 이벤트가 발생했을 때(키보드 입력 등) 사용자 정의 동작을 수행.

---

# hello triangle


## 셰이더 코드 작성 방식과 이유

셰이더(Shader)는 GPU에서 실행되는 작은 프로그램입니다. OpenGL에서 사용하는 셰이더 코드는 **GLSL (OpenGL Shading Language)** 라는 언어로 작성됩니다. 주어진 코드는 Vertex Shader로, 각 정점(Vertex)의 위치를 처리하는 역할을 합니다.

셰이더 코드는 보통 아래 이유로 이런 방식으로 작성됩니다:

1. GLSL 코드 자체를 문자열로 작성

const char *vertexShaderSource = "#version 330 core\n"

•셰이더는 GPU에서 실행되며, OpenGL은 GLSL 소스 코드를 문자열 형태로 전달받습니다. 따라서 GLSL 코드는 C++ 코드에서 문자열로 작성됩니다.
•#version 330 core는 GLSL 버전을 지정합니다. 여기서는 OpenGL 3.3에 맞는 core profile을 사용하고 있습니다.

2. Vertex Shader의 입력 변수 정의

"layout (location = 0) in vec3 aPos;\n"

•layout(location = 0):
•이 줄은 입력 변수의 **속성 위치(attribute location)** 를 지정합니다. 여기서는 위치가 0인 정점 데이터를 받습니다.
•OpenGL에서 정점 데이터를 사용할 때, 어떤 데이터가 어떤 속성(attribute)에 매핑될지 알려줘야 합니다.
•이 예에서는 **aPos** 라는 3D 벡터(vec3)를 입력으로 받아들입니다.
•in vec3 aPos;:
•Vertex Shader가 GPU로부터 입력으로 받는 데이터입니다.
•여기서는 정점의 위치 데이터를 aPos라는 변수로 전달받습니다.

3. 메인 함수와 OpenGL의 내장 변수 사용

"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

•void main():
•GLSL 셰이더는 항상 main 함수를 기준으로 실행됩니다. OpenGL이 자동으로 이 함수를 호출합니다.
•gl_Position:
•OpenGL의 내장 변수로, GPU가 처리한 결과를 클립 공간(Clip Space) 좌표로 변환하여 저장해야 합니다.
•이 변수는 Vertex Shader에서 필수로 설정해야 하는 출력 값입니다.
•여기서는 입력으로 받은 정점 데이터 aPos를 4D 벡터(vec4)로 변환하여 gl_Position에 저장합니다. 마지막 값 1.0은 **동차 좌표(homogeneous coordinates)** 를 위한 것입니다.
•OpenGL은 이 값을 받아 후속 단계(뷰 변환, 투영 변환, 클리핑 등)를 처리합니다.

## 셰이더 코드를 문자열로 끝내는 이유

"}\0";

•"\0":
•문자열 끝에 널 문자(null character)를 추가하는 것은 C 스타일 문자열에서 필수입니다.
•OpenGL의 glShaderSource 함수는 이 문자열을 GLSL 소스 코드로 받아들이기 때문에, 문자열이 올바르게 끝나는 것이 중요합니다.

왜 Vertex Shader를 이렇게 작성하는가?
1.GPU와 CPU의 역할 분담:
•셰이더는 GPU에서 실행되고, GLSL 코드는 GPU에서 실행될 프로그램을 정의합니다.
•이 방식으로 정점 데이터의 변환을 GPU에 맡겨 CPU의 부하를 줄일 수 있습니다.
2.범용적인 처리:
•Vertex Shader는 정점 데이터를 처리하는 가장 기본적인 역할을 합니다.
•여기서는 단순히 입력받은 정점 위치(aPos)를 클립 공간 좌표로 변환합니다.
3.OpenGL 파이프라인 규칙:
•OpenGL 파이프라인의 첫 단계는 정점 처리(Vertex Processing)입니다.
•정점 위치는 **gl_Position** 에 반드시 설정해야 하며, 그렇지 않으면 렌더링되지 않습니다.

Vertex Shader 코드의 핵심 내용
1.OpenGL GLSL 버전 명시 (#version 330 core): OpenGL 3.3 Core Profile에 맞춘 코드.
2.정점 데이터 입력 (layout(location = 0) in vec3 aPos): 정점의 위치 데이터 입력.
3.위치 변환 (gl_Position): 정점 데이터를 클립 공간으로 변환.
4.C++ 코드와 연동: 셰이더 소스는 문자열로 작성되어 OpenGL API (glShaderSource)로 전달됨.


## 여러개의 VAO를 만들 땐 언바인딩을 바로바로 해줄 필요가 없는 이유는?
   
“언바인딩할 필요가 없다”는 말은, OpenGL에서는 특정 VAO (Vertex Array Object)를 해제하거나 언바인딩(glBindVertexArray(0))하지 않아도 문제가 없다는 뜻입니다. 이유는 코드의 다음 부분에서 새로운 VAO를 바로 바인딩하기 때문입니다.

더 자세히 설명하자면:
1.VAO 바인딩이란?
VAO는 OpenGL에서 정점 데이터(Vertex Data)를 저장하는 객체입니다. glBindVertexArray를 호출하면 특정 VAO가 활성 상태(active)가 되어, 이후의 정점 설정 작업이 그 VAO에 저장됩니다.
2.언바인딩을 왜 하는가?
일반적으로 glBindVertexArray(0)를 호출하여 VAO를 언바인딩하는 이유는, 이후의 작업에서 다른 VAO가 의도치 않게 영향을 받지 않도록 하기 위해서입니다. 하지만…
3.언바인딩할 필요가 없는 경우:
다음 코드가 새로운 VAO를 바인딩하는 경우, 현재 활성 상태인 VAO는 덮어씌워지기 때문에 굳이 먼저 언바인딩하지 않아도 됩니다.
즉, 기존 VAO를 언바인딩한 뒤(glBindVertexArray(0)) 새로운 VAO를 바인딩하는 작업은 비효율적이며, 이를 생략할 수 있다는 의미입니다.

예시로 비교:

-  굳이 언바인딩하는 경우 (불필요):

glBindVertexArray(1); // 첫 번째 VAO 바인딩
// ...작업 수행...
glBindVertexArray(0); // 언바인딩
glBindVertexArray(2); // 새로운 VAO 바인딩

- 언바인딩 없이 바로 바인딩하는 경우 (효율적):

glBindVertexArray(1); // 첫 번째 VAO 바인딩
// ...작업 수행...
glBindVertexArray(2); // 새로운 VAO를 바로 바인딩 (이전 VAO는 자동으로 비활성화됨)

따라서, 이 경우 코드에 있는 주석처럼 굳이 언바인딩할 필요가 없다는 것이죠. OpenGL은 이전 VAO를 자동으로 비활성화하고 새로운 VAO를 활성화합니다.

새로운 VAO를 바로 바인딩(glBindVertexArray)하기만 하면, 현재 활성화된 VAO는 자동으로 비활성화되기 때문에 glBindVertexArray(0)로 명시적으로 언바인딩할 필요가 없습니다. OpenGL은 새로운 VAO를 바인딩하면 기존 VAO와의 연결을 자동으로 해제하거든요.

정리하자면:
•glBindVertexArray(0)는 VAO를 명시적으로 언바인딩하는 작업인데,
새로운 VAO를 곧바로 바인딩할 예정이라면 이 과정을 생략해도 무방합니다.
•한 번에 하나의 VAO만 활성화되므로, 이전 VAO를 굳이 언바인딩하지 않아도, 새 VAO를 바인딩하면 이전 VAO는 자동으로 비활성화됩니다.

예시로 다시 보자면:

glBindVertexArray(vao1); // VAO 1 활성화
// 작업 수행...
glBindVertexArray(vao2); // VAO 2를 바로 바인딩, VAO 1은 자동으로 비활성화됨

위 코드에서는 glBindVertexArray(0)를 호출할 필요가 없죠.
언바인딩을 꼭 해야 할 상황은 모든 VAO를 비활성화하고 아무것도 바인딩하지 않는 상태로 돌아가야 할 때뿐입니다.


## 셰이더 프로그램과 정점 객체사이의 관계

**glBufferData** 와 셰이더는 GPU에서 작동하는 서로 다른 단계에 있지만, 둘 다 GPU 상에서의 작업이기 때문에 밀접한 관련이 있습니다. 이를 단계적으로 설명하자면:

1. glBufferData의 역할: 정점 데이터를 GPU로 전송  
•glBufferData는 CPU에서 준비한 **정점 데이터(vertex data)** 를 GPU로 전송하여 GPU가 처리할 수 있도록 **버퍼 객체(Buffer Object)** 에 저장합니다.  
•이 데이터는 주로 정점의 위치(aPos), 색상 또는 텍스처 좌표 같은 정보를 포함합니다.  
•이 과정에서 OpenGL의 버퍼 메모리가 사용되며, GL_ARRAY_BUFFER(주로 정점 데이터를 위한 버퍼)에 데이터를 로드합니다.

### 예:

```cpp
float vertices[] = {
    // x, y, z 좌표
    0.5f,  0.5f, 0.0f,  // 오른쪽 위
    0.5f, -0.5f, 0.0f,  // 오른쪽 아래
   -0.5f, -0.5f, 0.0f,  // 왼쪽 아래
   -0.5f,  0.5f, 0.0f   // 왼쪽 위
};


glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
```

위 코드는 vertices 배열에 정의된 정점 데이터를 GPU로 전송하는 예입니다.

2. 셰이더의 역할: GPU에서 데이터 처리

셰이더는 GPU에서 실행되는 작은 프로그램으로, glBufferData로 전송된 데이터를 활용하여 그래픽 처리를 합니다.


•정점 셰이더(Vertex Shader):  
•GPU에 저장된 정점 데이터를 가져와 화면상의 위치(gl_Position)로 변환합니다.  
•이 과정에서 OpenGL의 **좌표 변환 과정(모델 → 뷰 → 투영)** 이 포함됩니다.  
•프래그먼트 셰이더(Fragment Shader):  
•정점 데이터를 기반으로 최종 픽셀의 색상, 텍스처 등을 계산합니다.


관련된 데이터 흐름:  
•정점 데이터(위치, 색상, 텍스처 좌표 등)는 glBufferData를 통해 GPU로 전송됩니다.  
•이 데이터는 셰이더 프로그램에서 **속성(Attribute)** 로 받아와 처리됩니다. 

### 예:

```cpp
layout (location = 0) in vec3 aPos; // 정점의 위치
layout (location = 1) in vec3 aColor; // 정점의 색상
```

3. 둘 사이의 연결: 속성 포인터와 glVertexAttribPointer  
•GPU로 보낸 데이터가 셰이더에서 사용되려면, OpenGL이 “어떤 데이터가 어떤 속성에 해당하는지” 알려줘야 합니다.  
•**glVertexAttribPointer** 를 사용해 이를 연결합니다.

### 예:

```cpp
// 정점 위치 데이터 (aPos)와 셰이더의 위치 속성 연결
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);
```


•여기서 0은 셰이더의 layout(location = 0)와 매칭됩니다.  
•이렇게 해야 glBufferData로 GPU에 전송된 데이터가 셰이더 프로그램으로 전달됩니다.

4. 요약: 데이터 흐름  
1.glBufferData: CPU → GPU로 데이터를 전송 (버퍼에 저장).
2.glVertexAttribPointer: 데이터와 셰이더 속성을 연결.  
3.셰이더: GPU에서 데이터를 받아 화면에 렌더링.

데이터의 전체 흐름을 그림으로 비유하자면:  
1.glBufferData: 데이터를 트럭에 실어 GPU 창고로 보낸다.  
2.glVertexAttribPointer: GPU 창고에서 셰이더 프로그램이 필요한 데이터를 꺼내는 방법을 지정한다.  
3.셰이더: 데이터(정점, 색상, 텍스처 등)를 가공하고 화면에 그린다.  



## 최종 흐름


### 여러분의 이해  
1. vertices 데이터:  
•**vertices** 는 화면에 그릴 정점 데이터를 의미합니다. 이 데이터는 일반적으로 정점의 위치, 색상, 텍스처 좌표 등을 포함합니다.  

2. glBufferData로 GPU로 전달:  
glBufferData 함수는 vertices 데이터를 GPU의 메모리로 보내는 역할을 합니다. 즉, CPU에서 GPU로 데이터를 전송하는 단계입니다. 

3. GPU 프로그램인 셰이더에서 aPos가 vertices였던 것:  
•정확히 맞습니다! 셰이더 프로그램 내에서 **aPos**는 정점 셰이더에서 vertices 데이터의 위치를 받아 처리합니다. **aPos**는 셰이더의 변수로, 이 변수에 vertices 데이터가 바인딩되며, 이 데이터는 **glVertexAttribPointer**를 통해 연결됩니다. 

4. glVertexAttribPointer로 속성 연결:  
•맞습니다! **glVertexAttribPointer**는 VAO와 VBO에서 정의된 정점 속성(위치, 색상 등)을 셰이더의 변수와 연결합니다. 예를 들어, **aPos**는 **glVertexAttribPointer**를 통해 vertices 데이터를 받는 셰이더의 변수로 연결됩니다.  
5. 프래그먼트 셰이더로 전달:  
•여기서 중요한 점은 프래그먼트 셰이더는 주로 색상 계산을 처리하는 부분입니다. **정점 셰이더에서 계산된 데이터(예: 정점 위치)**는 보통 프래그먼트 셰이더로 전달되어 최종적으로 화면에 그려지는 색상을 계산합니다. 즉, 프래그먼트 셰이더는 화면의 최종 색상을 결정하는 데 중요한 역할을 하며, aPos(정점 위치) 같은 데이터는 필요에 따라 프래그먼트 셰이더에서 사용될 수 있습니다.

### 전체 흐름  
1.vertices 데이터 -> **glBufferData**로 GPU로 전송.  
2.**glVertexAttribPointer**로 셰이더의 변수(aPos)와 정점 데이터를 연결.  
3.셰이더 프로그램에서 **aPos**를 사용하여 정점 변환(예: 좌표 변환).  
4.프래그먼트 셰이더에서 화면의 색상 계산.  

개선할 수 있는 부분  
•프래그먼트 셰이더에서 aPos가 직접적으로 사용되는 경우는 드물다는 점을 명확히 하면 좋습니다. **aPos**는 정점 셰이더에서 변환된 결과가 프래그먼트 셰이더로 전달되어 픽셀 단위 색상 계산에 사용되기도 합니다. 보통은 **fragColor**와 같은 색상 관련 변수들이 주로 프래그먼트 셰이더에서 다뤄집니다.

### 결론

전반적으로 잘 이해하셨고, 추가적인 세부 사항으로 프래그먼트 셰이더의 역할을 명확히 하면 더욱 완벽하게 이해할 수 있습니다. 아주 좋은 접근입니다!



 




# camera

### 오일러 각도 : 왜 두 값을 곱하는 걸까?

`direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));`  

3D 공간에서 카메라의 방향을 결정할 때,  
1. yaw(야우) → 좌우 회전 (XZ 평면에서 방향 결정)  
2. pitch(피치) → 상하 회전 (XY 평면에서 방향 결정)  

이 두 가지를 모두 고려해야 한다. 오른손 좌표계를 기억하면서 그 축들을 기준으로 회전하는것을 기억.

`cos(yaw): XZ 평면에서 좌우 방향 결정`  
cos(yaw): X축으로 이동하는 비율을 결정  
sin(yaw): Z축으로 이동하는 비율을 결정  
즉, cos(yaw) 값만 사용하면 XZ 평면에서의 방향만 반영됩니다.  

`cos(pitch): 상하 방향을 고려한 수평 투영량 조절`   
pitch(피치)은 카메라가 위/아래를 바라보는 정도를 나타냅니다.  
pitch가 커질수록(위쪽을 볼수록) XZ 평면에서의 이동이 줄어들어야 합니다.  
cos(pitch)는 XZ 평면으로 투영된 이동량을 결정합니다.  

🔹 예제 상황

🔹 pitch = 0° (정면을 바라봄)  
•cos(0°) = 1이므로 `cos(yaw) * cos(pitch) = cos(yaw)`.  
•X축 방향 이동이 그대로 적용됨.  

🔹 pitch = 90° (완전히 위를 봄)  
•cos(90°) = 0이므로 `cos(yaw) * cos(pitch) = 0`.  
•X축 이동이 없어지고, 오직 위(Y축)만 바라봄.  

🔹 pitch = -90° (완전히 아래를 봄)  
•cos(-90°) = 0이므로 마찬가지로 X축 이동 없음.  

즉, 카메라가 위쪽을 볼수록 X축과 Z축으로 이동하는 성분이 줄어들도록 cos(pitch)를 곱하는 것.  



## offset 계산순서가 다른 이유는?
`xoffset`과 `yoffset`의 계산 순서가 다른 이유는 OpenGL (정확히는 GLFW) 좌표계와 우리가 일반적으로 생각하는 화면 좌표계의 차이 때문이다. 좀 더 자세히 설명하면 다음과 같다.

**1. OpenGL/GLFW 좌표계:**

*   OpenGL (GLFW도 마찬가지)에서 화면의 왼쪽 아래 모서리가 원점 (0, 0)이다.
*   x축은 오른쪽으로 갈수록 증가한다.
*   y축은 *위쪽으로* 갈수록 증가한다.  이 점이 중요하다.

**2. 일반적인 화면 좌표계:**

*   일반적으로 화면 좌표계 (예: 그림판)에서는 화면의 왼쪽 *위* 모서리가 원점 (0, 0)이다.
*   x축은 오른쪽으로 갈수록 증가한다.
*   y축은 *아래쪽으로* 갈수록 증가한다.

**3. `xoffset` 계산:**

*   `float xoffset = xpos - lastX;`
*   `xpos`는 현재 마우스의 x 좌표이고, `lastX`는 이전 마우스의 x 좌표이다.
*   마우스가 오른쪽으로 움직였다면 `xpos`가 `lastX`보다 크므로 `xoffset`은 양수가 된다. 이는 OpenGL 좌표계에서 오른쪽으로 이동했음을 의미하므로 올바른 계산이다.

**4. `yoffset` 계산:**

*   `float yoffset = lastY - ypos;`
*   만약 `yoffset = ypos - lastY;` 로 계산했다면, 마우스가 위쪽으로 움직였을 때 `ypos`가 `lastY`보다 커지므로 `yoffset`은 양수가 된다.
*   하지만 OpenGL 좌표계에서는 위쪽으로 움직일수록 y 좌표가 *증가*하므로, 이는 우리가 원하는 결과가 아니다.  마우스가 위로 움직였다면 `yoffset`은 *음수*가 되어야 한다.
*   따라서 `lastY - ypos`로 계산하여 마우스가 위로 움직일 때 `yoffset`이 음수가 되도록 보정하는 것이다.  즉, y축의 방향을 뒤집어 주는 것이다.

**5. 요약:**

`xoffset = xpos - lastX;` 는 마우스의 수평 이동 방향을 올바르게 계산한다.

`yoffset = lastY - ypos;` 는 OpenGL 좌표계와 일반적인 화면 좌표계의 y축 방향이 반대이기 때문에, 마우스의 수직 이동 방향을 보정하기 위해 계산 순서를 뒤집은 것이다.

만약 OpenGL 좌표계가 아닌 다른 좌표계를 사용한다면, `yoffset` 계산 순서가 달라질 수 있다. 중요한 것은 좌표계에 맞춰 마우스 이동 방향을 정확하게 계산하는 것이다.






