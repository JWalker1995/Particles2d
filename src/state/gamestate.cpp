#include "gamestate.h"

/*
double m_pi = atan(1.0) * 4.0;
double m_pi_d2 = m_pi / 2.0;
*/

GameState::GameState(int seed)
{
    world = new World(seed, VERSION_CURRENT);
/*
    std::vector<GLenum> enables;
    enables.push_back(GL_BLEND);
    enables.push_back(GL_COLOR_LOGIC_OP);
    enables.push_back(GL_CULL_FACE);
    enables.push_back(GL_DEBUG_OUTPUT);
    enables.push_back(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    enables.push_back(GL_DEPTH_CLAMP);
    enables.push_back(GL_DEPTH_TEST);
    enables.push_back(GL_DITHER);
    enables.push_back(GL_FRAMEBUFFER_SRGB);
    enables.push_back(GL_LINE_SMOOTH);
    enables.push_back(GL_MULTISAMPLE);
    enables.push_back(GL_POLYGON_OFFSET_FILL);
    enables.push_back(GL_POLYGON_OFFSET_LINE);
    enables.push_back(GL_POLYGON_OFFSET_POINT);
    enables.push_back(GL_POLYGON_SMOOTH);
    enables.push_back(GL_PRIMITIVE_RESTART);
    enables.push_back(GL_PRIMITIVE_RESTART_FIXED_INDEX);
    enables.push_back(GL_RASTERIZER_DISCARD);
    enables.push_back(GL_SAMPLE_ALPHA_TO_COVERAGE);
    enables.push_back(GL_SAMPLE_ALPHA_TO_ONE);
    enables.push_back(GL_SAMPLE_COVERAGE);
    enables.push_back(GL_SAMPLE_SHADING);
    enables.push_back(GL_SAMPLE_MASK);
    enables.push_back(GL_SCISSOR_TEST);
    enables.push_back(GL_STENCIL_TEST);
    enables.push_back(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    enables.push_back(GL_PROGRAM_POINT_SIZE);
    std::vector<std::string> enable_strs;
    enable_strs.push_back("GL_BLEND");
    enable_strs.push_back("GL_COLOR_LOGIC_OP");
    enable_strs.push_back("GL_CULL_FACE");
    enable_strs.push_back("GL_DEBUG_OUTPUT");
    enable_strs.push_back("GL_DEBUG_OUTPUT_SYNCHRONOUS");
    enable_strs.push_back("GL_DEPTH_CLAMP");
    enable_strs.push_back("GL_DEPTH_TEST");
    enable_strs.push_back("GL_DITHER");
    enable_strs.push_back("GL_FRAMEBUFFER_SRGB");
    enable_strs.push_back("GL_LINE_SMOOTH");
    enable_strs.push_back("GL_MULTISAMPLE");
    enable_strs.push_back("GL_POLYGON_OFFSET_FILL");
    enable_strs.push_back("GL_POLYGON_OFFSET_LINE");
    enable_strs.push_back("GL_POLYGON_OFFSET_POINT");
    enable_strs.push_back("GL_POLYGON_SMOOTH");
    enable_strs.push_back("GL_PRIMITIVE_RESTART");
    enable_strs.push_back("GL_PRIMITIVE_RESTART_FIXED_INDEX");
    enable_strs.push_back("GL_RASTERIZER_DISCARD");
    enable_strs.push_back("GL_SAMPLE_ALPHA_TO_COVERAGE");
    enable_strs.push_back("GL_SAMPLE_ALPHA_TO_ONE");
    enable_strs.push_back("GL_SAMPLE_COVERAGE");
    enable_strs.push_back("GL_SAMPLE_SHADING");
    enable_strs.push_back("GL_SAMPLE_MASK");
    enable_strs.push_back("GL_SCISSOR_TEST");
    enable_strs.push_back("GL_STENCIL_TEST");
    enable_strs.push_back("GL_TEXTURE_CUBE_MAP_SEAMLESS");
    enable_strs.push_back("GL_PROGRAM_POINT_SIZE");
    int enable_i = 0;
*/

    glClearColor(0.0, 0.0, 0.2, 0.0);

    glDisable(GL_DEPTH_TEST);

    // Create light components.
    GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8, 1.0f };
    GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat position[] = { 0.0f, 0.0f, 0.0f, 1.0f };

    // Assign created components to GL_LIGHT0
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
}

GameState::~GameState()
{
    delete world;
}

void GameState::init()
{
#ifdef DISABLE_CURSOR
    glfwSetInputMode(get_glfw_window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);// GLFW_CURSOR_NORMAL, GLFW_CURSOR_HIDDEN, or GLFW_CURSOR_DISABLED
#endif
}

State *GameState::tick(float time)
{
    if (glfwGetKey(get_glfw_window(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(get_glfw_window(), true);
        return 0;
    }

    double cursor_x, cursor_y;
    glfwGetCursorPos(get_glfw_window(), &cursor_x, &cursor_y);

    if (glfwGetKey(get_glfw_window(), GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(get_glfw_window(), GLFW_KEY_W) == GLFW_PRESS)
    {
    }
    if (glfwGetKey(get_glfw_window(), GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(get_glfw_window(), GLFW_KEY_S) == GLFW_PRESS)
    {
    }
    if (glfwGetKey(get_glfw_window(), GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(get_glfw_window(), GLFW_KEY_A) == GLFW_PRESS)
    {
    }
    if (glfwGetKey(get_glfw_window(), GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(get_glfw_window(), GLFW_KEY_D) == GLFW_PRESS)
    {
    }

    int width = get_window()->get_width();
    int height = get_window()->get_height();
    float ratio = (float) width / (float) height;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, ratio, 1, 1000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //gluLookAt(player_pos.x, player_pos.y, player_pos.z, player_pos.x + player_orient.x, player_pos.y + player_orient.y, player_pos.z + player_orient.z, 0.0, 1.0, 0.0);

    world->tick(time);

    return 0;
}
