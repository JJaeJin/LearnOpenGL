#include <glad/glad.h>
#include "vendors/GLFW/glfw3.h"
#include <iostream>

using namespace std;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   " FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                   "}\0";
const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 " gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 "}\0";
// float vertices[] = {
//     -0.5f, -0.5f, 0.0f, // left
//     0.5f, -0.5f, 0.0f,  // right
//     0.0f, 0.5f, 0.0f    // top
// };

float vertices[] = {
    0.5f, 0.5f, 0.0f,   // top right
    0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f, // bottom left
    -0.5f, 0.5f, 0.0f   // top left
};

unsigned int indices[] = {
    0, 1, 3,
    1, 2, 3};

int main(void)
{
    glfwInit();
    // glfw 사전 설정 (3.3버전 + 코어 프로파일 버전)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // for Mac OS X

    // window 크기, 이름
    GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        cout << "Failed to create GLFW window\n";
        // 사용한 리소스 정리, glfw 종료
        glfwTerminate();
        return -1;
    }

    // window에 해당하는 openGL context를 현재 쓰래드에 활성화
    glfwMakeContextCurrent(window);

    // initialize GLAD (openGL 함수들 호출하기 전에 GLAD 초기화)
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    // 창의 크기가 바뀌면 호출될 콜백함수 등록
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // 프레임버퍼의 창 크기를 얻어와 glViewport() 설정
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    // [Vertex Shader] 생성 및 컴파일
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // [Fragment Shader] 생성 및 컴파일
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // [Shader Program] 생성 및 링크
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // [VBO] vertex buffer object 생성
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    // VBO 바인딩
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // 정점 데이터 전송
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 정점 속성 데이터 설정
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    // glEnableVertexAttribArray(0);

    // [VAO] vertex array object 생성, 바인딩
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    // VAO 바인딩
    glBindVertexArray(VAO);
    // VBO 복사
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 정점 속성 데이터 설정
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // [EOB] element buffer object
    unsigned int EBO;
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // VBO, VAO 연결 해제
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // 렌더링 loop (창이 닫히지 않는 동안)
    while (!glfwWindowShouldClose(window))
    {
        // input 처리
        processInput(window);

        // glClear에서 프레임버퍼 초기화 할 색상을 설정
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // 프레임버퍼를 초기화
        glClear(GL_COLOR_BUFFER_BIT);

        // 셰이더 프로그램 사용
        glUseProgram(shaderProgram);

        // 렌더링
        glBindVertexArray(VAO);
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // 더블 버퍼 스왑
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // 링크된 셰이더 삭제
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    // glfw 할당 해제
    glfwTerminate();
    return 0;
}

// 창 크기가 바뀔 때를 위한 콜백함수
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// window에서 입력에 대한 처리 함수
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}