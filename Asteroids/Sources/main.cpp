// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sstream>

#include "camera.hpp"

#include "textRenderer.hpp"
#include "spaceShip.hpp"
#include "projectile.hpp"
#include "asteroid.hpp"
#include "textRenderer.hpp"

using namespace TextRenders;
using namespace Asteroids;
using namespace Projectiles;
using namespace SpaceShips;

TextRenderer *textDisplay;

float shipRadius = 0.20;

const float mWidth = 1024;
const float mHeight = 578;

float lastKeyPressTimestamp = 0;

const int GAME_MAX_DIFFFICULTY = 2;
const int GAME_MAX_LIVES = 3;
const float MENU_KEY_PRESS_COOLDOWN = 0.2;
const int GAME_SUPER_PROJECTILES = 10;

int lives = GAME_MAX_LIVES;
int superProjectiles = GAME_SUPER_PROJECTILES;

SpaceShips::SpaceShip *spaceShip;

Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));
int score = 0;

enum GameState
{
    GAME_ACTIVE,
    GAME_MENU,
    GAME_OVER
};

GameState State = GAME_MENU;
int difficulty = 1;

void transferData()
{
}

void updateGameState(GameState state)
{
    State = state;

    switch (state)
    {
    case GAME_ACTIVE:

        score = 0.0;
        lives = GAME_MAX_LIVES;
        superProjectiles = GAME_SUPER_PROJECTILES;
        spaceShip->xOffSet = 0;
        spaceShip->yOffSet = 0;
        break;
    case GAME_MENU:
        break;
    case GAME_OVER:
        break;
    }
}

void Init()
{
    Asteroids::loadModel("../Asteroids/Objects/asteroid/asteroid.obj");
    SpaceShips::loadModel("../Asteroids/Objects/spaceship/spaceship.obj");
    Projectiles::loadModel("../Asteroids/Objects/projectile/projectile.obj");
    spaceShip = new SpaceShip("../Asteroids/Shaders/space_ship.vs", "../Asteroids/Shaders/space_ship.fs");
}

void checkShipCollisions()
{
    vector<Asteroid *> *asteroids = getAsteroids();

    for (unsigned long int i = 0; i < asteroids->size(); i++)
    {

        float width = abs(asteroids->at(i)->xOffSet - spaceShip->xOffSet);
        float height = abs(asteroids->at(i)->yOffSet - spaceShip->yOffSet);

        float distance = sqrt(width * width + height * height);

        float minDistanceForImpact = asteroids->at(i)->hitBox.max.x + shipRadius;

        if (minDistanceForImpact > distance)
        {
            if (lives == 1)
                updateGameState(GAME_OVER);

            asteroids->erase(asteroids->begin() + i);

            lives--;
        }
    }
}

void checkProjectileCollisions()
{
    vector<Projectile *> *projectiles = Projectiles::getProjectiles();
    vector<Asteroid *> *asteroids = Asteroids::getAsteroids();

    vector<Asteroid *> aCollisions;
    vector<Projectile *> pCollisions;

    AABB pHitBox = projectileHitbox();

    for (long unsigned int i = 0; i < projectiles->size(); i++)
    {
        for (long unsigned int j = 0; j < asteroids->size(); j++)
        {

            try
            {
                float width = abs(projectiles->at(i)->xOffSet - asteroids->at(j)->xOffSet);
                float height = abs(projectiles->at(i)->yOffSet - asteroids->at(j)->yOffSet);

                float
                    distance = sqrt(width * width + height * height);

                float minDistanceForImpact = asteroids->at(i)->hitBox.max.x + pHitBox.max.x;

                if (minDistanceForImpact > distance)
                {
                    aCollisions.push_back(asteroids->at(j));
                    pCollisions.push_back(projectiles->at(i));
                }
            }

            catch (const std::out_of_range &e)
            {
            }
        }
    }

    for (Projectile *collision : pCollisions)
    {
        for (long unsigned int i = 0; i < projectiles->size(); i++)
        {
            if (projectiles->at(i) == collision)
            {
                projectiles->erase(projectiles->begin() + i);
                break;
            }
        }
    }

    for (Asteroid *collision : aCollisions)
    {
        for (long unsigned int i = 0; i < asteroids->size(); i++)
        {
            if (asteroids->at(i) == collision)
            {
                score += difficulty;
                asteroids->erase(asteroids->begin() + i);
                break;
            }
        }
    }
}

void checkAsteroidCollisions()
{
    vector<Asteroid *> *asteroids = getAsteroids();

    for (long unsigned int a1 = 0; a1 < asteroids->size(); a1++)
    {
        for (long unsigned int a2 = 0; a2 < asteroids->size(); a2++)
        {
            if (a1 <= a2)
                continue;

            float width = abs(asteroids->at(a1)->xOffSet - asteroids->at(a2)->xOffSet);
            float height = abs(asteroids->at(a1)->yOffSet - asteroids->at(a2)->yOffSet);

            float distance = sqrt(width * width + height * height);

            float minDistanceForImpact = asteroids->at(a1)->hitBox.max.x + asteroids->at(a2)->hitBox.max.x;

            if (minDistanceForImpact >= distance)
            {
                // FORMULA: V - 2*(V dot N)*N

                glm::vec2 vecA1 = glm::vec2(glm::cos(asteroids->at(a1)->angle), glm::sin(asteroids->at(a1)->angle));
                glm::vec2 vecA2 = glm::vec2(glm::cos(asteroids->at(a2)->angle), glm::sin(asteroids->at(a2)->angle));
                // Normalized Vectors
                glm::vec2 nVecA1 = glm::normalize(vecA1 - vecA2);
                glm::vec2 nVecA2 = glm::normalize(vecA2 - vecA1);

                // Alternative -> glm::reflect(vecA1, nVecA1)
                glm::vec2 newVecA1 = vecA1 - 2 * glm::dot(vecA1, nVecA1) * nVecA1;

                // Alternative -> glm::reflect(vecA2, nVecA2)
                glm::vec2 newVecA2 = vecA2 - 2 * glm::dot(vecA2, nVecA2) * nVecA2;

                // Update angles
                asteroids->at(a1)->angle = glm::atan(newVecA1.y, newVecA1.x);
                asteroids->at(a2)->angle = glm::atan(newVecA2.y, newVecA2.x);
            }
        }
    }
}

void Render()
{
    switch (State)
    {
    case GAME_ACTIVE:

        checkProjectileCollisions();
        checkAsteroidCollisions();
        checkShipCollisions();

        spaceShip->render(mWidth, mHeight, camera);
        Asteroids::renderAsteroids(mWidth, mHeight, difficulty, camera);
        Projectiles::renderProjectiles(mWidth, mHeight, camera);

        textDisplay->renderText(std::to_string(score), 10.0f, 10.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));

        textDisplay->renderText("Lives: " + std::to_string(lives), mWidth - 125.0f, 10.0f, 0.5f, glm::vec3(0.5, 0.5f, 0.0f));
        
        textDisplay->renderText("Special: " + std::to_string(superProjectiles), 10, 50.0f, 0.5f, glm::vec3(0.5, 0.8f, 0.2f));

        if (State == GAME_OVER)
        {
            Asteroids::destroyAll();
            Projectiles::destroyAll();
        }

        break;

    case GAME_MENU:

        textDisplay->renderText("Press ENTER to start", 250.0f, mHeight / 2, 1.0f);
        textDisplay->renderText("Press A or D to select level", 245.0f, mHeight / 2 - 40.0f, 0.75f);
        textDisplay->renderText(
            difficulty == 1 ? "Easy" : "Hard",
            440.0f,
            mHeight / 2 - 120.0f,
            1.5f,
            difficulty == 1 ? glm::vec3(0.0, 0.5, 0.0) : glm::vec3(0.5, 0.0, 0.0));
        break;

    case GAME_OVER:

        textDisplay->renderText("GAME OVER", 250.0f, mHeight / 2, 1.0f, glm::vec3(0.0, 0.5, 0.0));
        textDisplay->renderText("Final Score: " + std::to_string(score), 245.0f, mHeight / 2 - 40.0f, 0.75f);
        textDisplay->renderText(
            "Press R to go back to menu",
            245.0f,
            mHeight / 2 - 150.0f,
            0.5f,
            glm::vec3(0.5, 0.5, 0.0));
        break;

    default:
        break;
    }
}

void ProcessInput(GLFWwindow *window)
{
    int aux = 0;
    switch (State)
    {
    case GAME_ACTIVE:
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, true);
        }
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        {
            spaceShip->moveForward();
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        {
            spaceShip->moveBackward();
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        {
            spaceShip->rotateLeft();
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        {
            spaceShip->rotateRight();
        }
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        {
            Projectiles::fireProjectile(mWidth, mHeight, spaceShip->xOffSet, spaceShip->yOffSet, spaceShip->angle);
        }

        break;
    case GAME_MENU:

        if (glfwGetTime() < lastKeyPressTimestamp + MENU_KEY_PRESS_COOLDOWN)
            break;

        if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
        {
            updateGameState(GAME_ACTIVE);
            lastKeyPressTimestamp = glfwGetTime();
        }

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            difficulty > 1 ? difficulty-- : difficulty = GAME_MAX_DIFFFICULTY;
            lastKeyPressTimestamp = glfwGetTime();
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            difficulty < GAME_MAX_DIFFFICULTY ? difficulty++ : difficulty = 1;
            lastKeyPressTimestamp = glfwGetTime();
        }

        break;

    case GAME_OVER:

        if (glfwGetTime() < lastKeyPressTimestamp + MENU_KEY_PRESS_COOLDOWN)
            break;

        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        {
            updateGameState(GAME_MENU);
            lastKeyPressTimestamp = glfwGetTime();
        }

        break;
    }
}
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_S && action == GLFW_PRESS && superProjectiles != 0)
    {
        superProjectiles--;

        Projectiles::fireProjectile3(mWidth, mHeight, spaceShip->xOffSet, spaceShip->yOffSet, spaceShip->angle);
    }
}

int main()
{
    // initialize random seed
    srand(time(NULL));

    // Load GLFW and Create a Window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

// GLFW_OPENGL_FORWARD_COMPAT Hint needs to be set on Darwin systems "Mac OS"
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow *window = glfwCreateWindow(mWidth, mHeight, "Asteroids", nullptr, nullptr);

    // Check for Valid Context
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Create Context and Load OpenGL Functions
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    textDisplay = new TextRenderer(mWidth, mHeight);

    Shader shader1("../Asteroids/Shaders/background.vs", "../Asteroids/Shaders/background.fs");
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions          // colors           // texture coords
        1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // top right
        1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
        -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // top left
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    unsigned char *data = stbi_load("/home/ferramenta/Documents/3º2º/CG/CG-Project/Asteroids/Sources/nebula.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // Loading models
    Init();

    // Rendering Loop
    while (!glfwWindowShouldClose(window))
    {

        // Input Control
        ProcessInput(window);

        // { Begin of Render Logic
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClearDepth(1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindTexture(GL_TEXTURE_2D, texture);

        // render container
        shader1.use();
        glm::mat4 modelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(6.0f, 6.0f, 0.0f));
        shader1.setMat4("modelMatrix", modelMatrix);
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), mWidth / mHeight, 0.1f, 1000.0f);
        projection = glm::translate(projection, glm::vec3(0.0, 0.0, -8.0f));

        glm::mat4 view = camera.GetViewMatrix();
        shader1.setMat4("projection", projection);
        shader1.setMat4("view", view);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        Render();
        // } End of Render Logic

        // Swap Buffers and Draw
        glfwSwapBuffers(window);
        // Check and Call Events
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return EXIT_SUCCESS;
}
