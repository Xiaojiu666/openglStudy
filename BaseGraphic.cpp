#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>


//窗口大小变化回调
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//键盘事件监听
void processInput(GLFWwindow* window);


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//顶点着色器
const char* vertexShaderSource = "#version 330 core\n"  //版本声明，同时明确表示我们会使用核心模式
"layout (location = 0) in vec3 aPos;\n"                 //in关键字设置输入变量  ，设定了输入变量的位置值为 0 ，创建一个vec3输入变量aPos
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"   //gl_Position的值为顶点着色器的输出
"}\0";

//片元着色器
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"                                 //out关键字设置输出变量
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

int main()
{

    glfwInit();  //初始化GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //上下文的客户端API版本 主版本号
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //上下文的客户端API版本 次版本号
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  //设置OpenGL使用的模式为核心模式 CORE_PROFILE
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //MAC OS X系统配置生效设置


    //创建窗口对象
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window); //通知GLFW将我们窗口的上下文设置为当前线程的主上下文

     //初始化GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    //注册 framebuffer_size_callback 这个函数，当窗口调整大小时调用 函数framebuffer_size_callback 
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);





    //创建顶点着色器
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); //把这个着色器源码附加到着色器对象上
    glCompileShader(vertexShader); //编译着色器对象

    //检查顶点着色器是否编译成功
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }




    // 创建片元着色器
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // 创建一个着色器程序对象
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);  //附加顶点着色器
    glAttachShader(shaderProgram, fragmentShader); //附加片元着色器
    glLinkProgram(shaderProgram);                  //链接着色器程序对象

    //检测链接着色器程序是否失败
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    //删除着色器对象，因为已经不需要了
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //三角形顶点数组
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // 点left  
         0.5f, -0.5f, 0.0f, // 点right 
         0.0f,  0.5f, 0.0f  // 点top   
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO); //创建顶点数组对象 VAO  ，缓冲ID为1 ，&VAO代表指向VAO变量的指针
    glGenBuffers(1, &VBO);  //创建一个VBO对象 ，缓冲ID为1
    glBindVertexArray(VAO); // 想使用VAO，要做的只是使用glBindVertexArray绑定VAO


    //把新创建的缓冲绑定到GL_ARRAY_BUFFER目标上 ，因为顶点缓冲对象VBO的缓冲类型是GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //把之前定义的顶点数据复制到缓冲的内存中，第四个参数控制是否把数据放在显卡可以告诉写入的内存部分
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    //告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上）
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //以顶点属性位置值作为参数，启用顶点属性；
    glEnableVertexAttribArray(0);

    // 注意，这是允许的，调用glVertexAttribPointer将VBO注册为顶点属性的绑定顶点缓冲区对象，这样我们就可以安全地解除绑定
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // 您可以在以后解除VAO的绑定，这样其他的VAO调用就不会意外地修改这个VAO，但是这种情况很少发生。修改otherVAOs需要调用glBindVertexArray，
    //所以我们通常不会在没有直接必要的时候取消绑定VAOs(或VBOs)。
    glBindVertexArray(0);


    // 使用线框模式绘制图元
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //当用户没有关闭窗口时执行循环渲染
    while (!glfwWindowShouldClose(window))
    {
        //键盘事件监听
        processInput(window);

        //清空屏幕的颜色缓冲
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //激活着色器程序对象，之后每个着色器调用和渲染调用都会使用这个着色器程序对象
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);            // 绑定VAO
        glDrawArrays(GL_TRIANGLES, 0, 3);  //绘制，使用当前激活的着色器，之前定义的顶点属性配置，和VBO的顶点数据（通过VAO间接绑定）来绘制图元
        // glBindVertexArray(0); // no need to unbind it every time 


        glfwSwapBuffers(window); //交换颜色缓冲
        glfwPollEvents(); //检查有没有触发什么事件（比如键盘输入、鼠标移动等）、更新窗口状态，并调用对应的回调函数
    }



    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);


    glfwTerminate(); //释放或删除之前的分配的所有资源
    return 0;
}


void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    //设置OpenGL渲染窗口（视口）的尺寸大小
    glViewport(0, 0, width, height);
}