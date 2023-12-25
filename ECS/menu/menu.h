#pragma ONCE
#include "../../init/utils.h"

enum AnswerEnum {
    NO_ANSWER = -1,
    START = 0,
    SET_LEVEL = 1,
    EXIT = 2,
    EXIT_TO_MENU = 3
};

class Menu {
public:
    Menu()
    {
        if (!backgroundTexture.loadFromFile("../sprites/menu/background.png"))
        {
            std::cout << "Error: I can't read texture \"../sprites/menu/background.png\"!!!\n";
            exit(1);
        }
        if (!mainMenuTexture.loadFromFile("../sprites/menu/menu_300x350.png"))
        {
            std::cout << "Error: I can't read texture \"../sprites/menu/menu_300x350.png\"!!!\n";
            exit(1);
        }
        if (!lostTexture.loadFromFile("../sprites/menu/lost.png"))
        {
            std::cout << "Error: I can't read texture \"../sprites/menu/lost.png\"!!!\n";
            exit(1);
        }
        if (!winTexture.loadFromFile("../sprites/menu/win.png"))
        {
            std::cout << "Error: I can't read texture \"../sprites/menu/win.png\"!!!\n";
            exit(1);
        }
        if (!setLevelTexture.loadFromFile("../sprites/menu/levels_250x550.png"))
        {
            std::cout << "Error: I can't read texture \"../sprites/menu/levels_250x550.png\"!!!\n";
            exit(1);
        }
        if (!previewTexture.loadFromFile("../sprites/menu/preview_300x225.png"))
        {
            std::cout << "Error: I can't read texture \"../sprites/menu/preview_300x225.png\"!!!\n";
            exit(1);
        }
        if (!cursorTexture.loadFromFile("../sprites/bullet_enemy.png"))
        {
            std::cout << "Error: I can't read texture \"sprites/bullet_enemy.png\"!!!\n";
            exit(1);
        }
        for (int i = 0; i < 10; i++)
        {
            auto num = std::to_string(i + 1);
            auto fileName = "../sprites/menu/levels/level_" + num + ".png";
            if (!levelTextures[i].loadFromFile(fileName))
            {
                std::cout << "Error: I can't read texture \"" << fileName << "\"!!!\n";
                exit(1);
            }
        }

        background.setTexture(backgroundTexture);
        mainMenu.setTexture(mainMenuTexture);
        lost.setTexture(lostTexture);
        win.setTexture(winTexture);
        setLevel.setTexture(setLevelTexture);
        preview.setTexture(previewTexture);
        cursor.setTexture(cursorTexture);

        setOriginUpCenter(cursor);
        setDirection(cursor, RIGHT);

        background.setPosition({0.f, 0.f});
        mainMenu.setPosition({(WINDOW_WIDTH - mainMenu.getLocalBounds().width) / 2, (WINDOW_HEIGHT - mainMenu.getLocalBounds().height) / 2});
        lost.setPosition({(WINDOW_WIDTH - lost.getLocalBounds().width) / 2, (WINDOW_HEIGHT - lost.getLocalBounds().height) / 2});
        win.setPosition({(WINDOW_WIDTH - win.getLocalBounds().width) / 2, (WINDOW_HEIGHT - win.getLocalBounds().height) / 2});
        setLevel.setPosition({100.f, 25.f});
        preview.setPosition({400.f, 185.f});
        levelPreview.setPosition({415.f, 195.f});
        levelPreview.setTexture(levelTextures[LEVEL]);
        setScalePreview(levelPreview);
    }
    sf::RenderWindow window;

    sf::Texture backgroundTexture;
    sf::Texture mainMenuTexture;
    sf::Texture lostTexture;
    sf::Texture winTexture;
    sf::Texture setLevelTexture;
    sf::Texture previewTexture;
    sf::Texture cursorTexture;
    sf::Texture levelTextures[10];

    sf::Sprite background;
    sf::Sprite mainMenu;
    sf::Sprite lost;
    sf::Sprite win;
    sf::Sprite setLevel;
    sf::Sprite preview;
    sf::Sprite cursor;
    sf::Sprite levelPreview;

    void checkEvent(AnswerEnum& answer)
    {
        sf::Event event = {};
        while(window.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::Closed:
                    answer = EXIT;
                    window.close();
                    break;
                default:
                    break;
            }
        }
    }

    AnswerEnum showMainMenu()
    {
        AnswerEnum answer = NO_ANSWER;
        const float CURSOR_OFFSET_X = 50.f;
        const float CURSOR_OFFSET_Y = 120.f;

        sf::Vector2f positionMenu = mainMenu.getPosition();

        float startPositionCursor = 0.f;
        float stepPositionCursor = 60.f;
        float endPositionCursor = stepPositionCursor * 2;
        float currentPositionCursor = startPositionCursor;

        sf::ContextSettings settings;
        settings.antialiasingLevel = 8;
        window.create(sf::VideoMode({ WINDOW_WIDTH, WINDOW_HEIGHT }),
                           "Tanks",
                           sf::Style::Default,
                           settings);

        sf::Vector2f positionCursor;
        positionCursor.x = positionMenu.x + CURSOR_OFFSET_X;
        positionCursor.y = positionMenu.y + CURSOR_OFFSET_Y + currentPositionCursor;

        while (answer == NO_ANSWER)
        {
            checkEvent(answer);
            window.clear();
            window.draw(background);
            window.draw(mainMenu);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
            {
                answer = AnswerEnum((currentPositionCursor - startPositionCursor) / stepPositionCursor);
                while (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {}
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            {
                if (currentPositionCursor == startPositionCursor)
                    currentPositionCursor = endPositionCursor;
                else
                    currentPositionCursor -= stepPositionCursor;
                while (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {}
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            {
                if (currentPositionCursor == endPositionCursor)
                    currentPositionCursor = startPositionCursor;
                else
                    currentPositionCursor += stepPositionCursor;
                while (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {}
            }
            positionCursor.y = positionMenu.y + CURSOR_OFFSET_Y + currentPositionCursor;
            cursor.setPosition(positionCursor);
            window.draw(cursor);
            window.display();
        }

        window.close();
        return answer;
    }

    AnswerEnum showYouWin()
    {
        AnswerEnum answer = NO_ANSWER;
        const float CURSOR_OFFSET_X = 45.f;
        const float CURSOR_OFFSET_Y = 85.f;

        sf::Vector2f positionMenu = win.getPosition();

        float startPositionCursor = 0.f;
        float stepPositionCursor = 40.f;
        float endPositionCursor = stepPositionCursor * 2;
        float currentPositionCursor = startPositionCursor;

        sf::ContextSettings settings;
        settings.antialiasingLevel = 8;
        window.create(sf::VideoMode({ WINDOW_WIDTH, WINDOW_HEIGHT }),
                      "Tanks",
                      sf::Style::Default,
                      settings);

        sf::Vector2f positionCursor;
        positionCursor.x = positionMenu.x + CURSOR_OFFSET_X;
        positionCursor.y = positionMenu.y + CURSOR_OFFSET_Y + currentPositionCursor;

        while (answer == NO_ANSWER)
        {
            checkEvent(answer);
            window.clear();
            window.draw(background);
            window.draw(win);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
            {
                answer = AnswerEnum((currentPositionCursor - startPositionCursor) / stepPositionCursor);
                if (answer == 0)
                {
                    if (LEVEL != 9)
                        LEVEL++;
                    else
                        LEVEL = 0;
                    answer = START;
                }
                else if (answer == 1)
                    answer = START;
                else if (answer == 2)
                    answer = EXIT_TO_MENU;
                while (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {}
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            {
                if (currentPositionCursor == startPositionCursor)
                    currentPositionCursor = endPositionCursor;
                else
                    currentPositionCursor -= stepPositionCursor;
                while (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {}
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            {
                if (currentPositionCursor == endPositionCursor)
                    currentPositionCursor = startPositionCursor;
                else
                    currentPositionCursor += stepPositionCursor;
                while (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {}
            }
            positionCursor.y = positionMenu.y + CURSOR_OFFSET_Y + currentPositionCursor;
            cursor.setPosition(positionCursor);
            window.draw(cursor);
            window.display();
        }

        window.close();
        return answer;
    }

    AnswerEnum showYouLost()
    {
        AnswerEnum answer = NO_ANSWER;
        const float CURSOR_OFFSET_X = 50.f;
        const float CURSOR_OFFSET_Y = 90.f;

        sf::Vector2f positionMenu = lost.getPosition();

        float startPositionCursor = 0.f;
        float stepPositionCursor = 50.f;
        float endPositionCursor = stepPositionCursor;
        float currentPositionCursor = startPositionCursor;

        sf::ContextSettings settings;
        settings.antialiasingLevel = 8;
        window.create(sf::VideoMode({ WINDOW_WIDTH, WINDOW_HEIGHT }),
                      "Tanks",
                      sf::Style::Default,
                      settings);

        sf::Vector2f positionCursor;
        positionCursor.x = positionMenu.x + CURSOR_OFFSET_X;
        positionCursor.y = positionMenu.y + CURSOR_OFFSET_Y + currentPositionCursor;

        while (answer == NO_ANSWER)
        {
            checkEvent(answer);
            window.clear();
            window.draw(background);
            window.draw(lost);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
            {
                answer = AnswerEnum((currentPositionCursor - startPositionCursor) / stepPositionCursor);
                if (answer == 1)
                    answer = EXIT_TO_MENU;
                while (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {}
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            {
                if (currentPositionCursor == startPositionCursor)
                    currentPositionCursor = endPositionCursor;
                else
                    currentPositionCursor -= stepPositionCursor;
                while (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {}
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            {
                if (currentPositionCursor == endPositionCursor)
                    currentPositionCursor = startPositionCursor;
                else
                    currentPositionCursor += stepPositionCursor;
                while (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {}
            }
            positionCursor.y = positionMenu.y + CURSOR_OFFSET_Y + currentPositionCursor;
            cursor.setPosition(positionCursor);
            window.draw(cursor);
            window.display();
        }

        window.close();
        return answer;
    }

    AnswerEnum showSetLevel()
    {
        AnswerEnum answer = NO_ANSWER;
        const float CURSOR_OFFSET_X = 110.f;
        const float CURSOR_OFFSET_Y = 170.f;

        sf::Vector2f positionMenu = setLevel.getPosition();

        float stepPositionCursor = 29.f;
        float startPositionCursor = 0.f;
        float endPositionCursor = stepPositionCursor * 9;
        float currentPositionCursor = startPositionCursor + stepPositionCursor * float(LEVEL);

        sf::ContextSettings settings;
        settings.antialiasingLevel = 8;
        window.create(sf::VideoMode({ WINDOW_WIDTH, WINDOW_HEIGHT }),
                      "Tanks",
                      sf::Style::Default,
                      settings);

        sf::Vector2f positionCursor;
        positionCursor.x = positionMenu.x + CURSOR_OFFSET_X;
        positionCursor.y = positionMenu.y + CURSOR_OFFSET_Y + currentPositionCursor;

        levelPreview.setTexture(levelTextures[LEVEL]);

        while (answer == NO_ANSWER)
        {
            checkEvent(answer);
            window.clear();
            window.draw(background);
            window.draw(setLevel);
            window.draw(preview);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
            {
                answer = START;
                while (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {}
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                answer = EXIT_TO_MENU;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            {
                if (currentPositionCursor == startPositionCursor)
                    currentPositionCursor = endPositionCursor;
                else
                    currentPositionCursor -= stepPositionCursor;

                LEVEL = int((currentPositionCursor - startPositionCursor) / stepPositionCursor);
                levelPreview.setTexture(levelTextures[LEVEL]);

                while (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {}
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            {
                if (currentPositionCursor == endPositionCursor)
                    currentPositionCursor = startPositionCursor;
                else
                    currentPositionCursor += stepPositionCursor;

                LEVEL = int((currentPositionCursor - startPositionCursor) / stepPositionCursor);
                levelPreview.setTexture(levelTextures[LEVEL]);

                while (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {}
            }
            window.draw(levelPreview);
            positionCursor.y = positionMenu.y + CURSOR_OFFSET_Y + currentPositionCursor;
            cursor.setPosition(positionCursor);
            window.draw(cursor);
            window.display();
        }

        window.close();
        return answer;
    }

    static void setScalePreview(sf::Sprite& sprite)
    {
        const sf::Vector2f targetSize = {270.f, 205.f};
        sf::FloatRect size = sprite.getGlobalBounds();
        sprite.setScale(
                targetSize.x / size.width,
                targetSize.y / size.height);
    }
};