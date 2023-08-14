//a test file to test out the engine and shit

#include "Core/App.hpp"


int main(){
    Forge::App app = Forge::App();

    app.CreateWindow(glm::vec2(1920, 1080), "example test", false);
}