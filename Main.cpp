#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode(640, 300), "SFML Piano");

    // Load font
    sf::Font font;//Font object is created
    if (!font.loadFromFile("../Fonts/arial.ttf")) {
        std::cerr << "Error: Could not load font!" << std::endl;
        return -1;
    }
    
    // Load sound buffers
    std::unordered_map<sf::Keyboard::Key, sf::Sound> sounds;
    std::vector<std::string> notes = {
        "../Music/C_s.wav", "../Music/D_s.wav", "../Music/F_s.wav", "../Music/G_s.wav", "../Music/Bb.wav",
        "../Music/C_s1.wav", "../Music/D_s1.wav", "../Music/C.wav", "../Music/D.wav", "../Music/E.wav",
        "../Music/F.wav", "../Music/G.wav", "../Music/A.wav", "../Music/B.wav", "../Music/C1.wav",
        "../Music/D1.wav", "../Music/E1.wav", "../Music/F1.wav"
    };

    std::vector<sf::Keyboard::Key> keys = {
        sf::Keyboard::W, sf::Keyboard::E, sf::Keyboard::T, sf::Keyboard::Y, sf::Keyboard::U,
        sf::Keyboard::O, sf::Keyboard::P, sf::Keyboard::A, sf::Keyboard::S, sf::Keyboard::D,
        sf::Keyboard::F, sf::Keyboard::G, sf::Keyboard::H, sf::Keyboard::J, sf::Keyboard::K,
        sf::Keyboard::L, sf::Keyboard::V, sf::Keyboard::B
    };

    std::vector<std::string> keyLabels = { "W", "E", "T", "Y", "U", "O", "P", "A", "S", "D",
        "F", "G", "H", "J", "K", "L", "V", "B" };

    std::vector<sf::SoundBuffer> buffers(notes.size());
    std::unordered_map<sf::Keyboard::Key, sf::RectangleShape> keyShapes;
    std::unordered_map<sf::Keyboard::Key, sf::Text> keyTexts;

    int keyWidth = 40, keyHeight = 200, blackKeyWidth = 30, blackKeyHeight = 120;//Key dimensions and positions

    // Black key positions
    std::unordered_map<sf::Keyboard::Key, float> blackKeyPositions = {
        {sf::Keyboard::W, 75}, {sf::Keyboard::E, 125}, {sf::Keyboard::T, 225},
        {sf::Keyboard::Y, 275}, {sf::Keyboard::U, 325}, {sf::Keyboard::O, 425},
        {sf::Keyboard::P, 475}
    };

    // White key positions
    std::unordered_map<sf::Keyboard::Key, float> keyPositions = {
        {sf::Keyboard::A, 50}, {sf::Keyboard::S, 100}, {sf::Keyboard::D, 150},
        {sf::Keyboard::F, 200}, {sf::Keyboard::G, 250}, {sf::Keyboard::H, 300},
        {sf::Keyboard::J, 350}, {sf::Keyboard::K, 400}, {sf::Keyboard::L, 450},
        {sf::Keyboard::V, 500}, {sf::Keyboard::B, 550}
    };

    // Load sounds and create keys
    for (size_t i = 0; i < notes.size(); ++i) {
        if (!buffers[i].loadFromFile(notes[i])) {
            std::cerr << "Error loading: " << notes[i] << std::endl;
            return -1;
        }
        sounds[keys[i]].setBuffer(buffers[i]);

        sf::RectangleShape key;
        bool isBlackKey = blackKeyPositions.find(keys[i]) != blackKeyPositions.end();

        if (isBlackKey) {
            key = sf::RectangleShape(sf::Vector2f(blackKeyWidth, blackKeyHeight));
            key.setPosition(blackKeyPositions[keys[i]], 50);
            key.setFillColor(sf::Color::Black);
        }
        else {
            key = sf::RectangleShape(sf::Vector2f(keyWidth, keyHeight));
            key.setPosition(keyPositions[keys[i]], 50);
            key.setFillColor(sf::Color::White);
        }
        key.setOutlineThickness(3);
        key.setOutlineColor(sf::Color::Black);
        keyShapes[keys[i]] = key; //Stores keys into  keyShapes vector

        // Add key labels
        sf::Text text;// Text object is created
        text.setFont(font);
        text.setString(keyLabels[i]);
        text.setCharacterSize(20);
        text.setFillColor(isBlackKey ? sf::Color::White : sf::Color::Black);
        text.setStyle(sf::Text::Bold);
        text.setPosition(key.getPosition().x + (key.getSize().x - text.getLocalBounds().width) / 2,
            key.getPosition().y + key.getSize().y - 30); //text label positioning
        keyTexts[keys[i]] = text; //Stores text of keys into keyTexts vector
    }

    while (window.isOpen()) { //Event 
        sf::Event event;
        while (window.pollEvent(event)) { //Pending events or queued up events
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed && sounds.find(event.key.code) != sounds.end()) {
                if (sounds[event.key.code].getStatus() != sf::Sound::Playing) { //Checks if the sound is not playing
                    sounds[event.key.code].play();
                }
                keyShapes[event.key.code].setFillColor(sf::Color(230,226,200));//Colour Shown On Pressing Keys
            }
            if (event.type == sf::Event::KeyReleased && keyShapes.find(event.key.code) != keyShapes.end()) {
                if (blackKeyPositions.find(event.key.code) != blackKeyPositions.end()) {
                    keyShapes[event.key.code].setFillColor(sf::Color::Black);
                }
                else {
                    keyShapes[event.key.code].setFillColor(sf::Color::White);
                }
            }
        }

        window.clear(sf::Color(191, 190, 207));//Background Color

        // Draw white keys first
        for (const auto& pair : keyShapes) {
            if (blackKeyPositions.find(pair.first) == blackKeyPositions.end()) {
                window.draw(pair.second);
            }
        }

        // Draw black keys on top
        for (const auto& pair : keyShapes) {
            if (blackKeyPositions.find(pair.first) != blackKeyPositions.end()) {
                window.draw(pair.second);
            }
        }

        // Draw labels
        for (const auto& pair : keyTexts) {
            window.draw(pair.second);
        }

        window.display();
    }

    return 0;
}