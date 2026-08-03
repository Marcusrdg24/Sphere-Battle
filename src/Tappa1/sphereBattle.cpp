#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/Event.hpp>
#include <vector>
#include <string>
#include <iostream>

////////////////////////
// Variabili Iniziali //
////////////////////////

const char* window_title = "Sphere Battle";
const unsigned window_width = 1000;
const unsigned window_height = 800;
const float max_frame_rate = 1;
int schermata = 0;


//////////
// Draw //
//////////

void drawTitle (sf::RenderWindow& window, const sf::Text& titleText, const sf::Text& subtitleText) {
    window.draw(titleText);
    window.draw(subtitleText);
}


////////////
// Handle //
////////////

void handle_close (sf::RenderWindow& window) {
    window.close();
}



////////////////
// Ausiliarie //
////////////////

sf::Font font_loader (sf::RenderWindow& window) {
    sf::Font font;
    if (!font.openFromFile("src/Utilities/GeistPixel-Regular-VariableFont_ELSH.ttf")) {
        std::cerr << "Errore nel caricamento del font!" << std::endl;
        window.close();
    }
    return font;
}

sf::Text title_text (sf::Font& font) {
    // Stile
    sf::Text titleText (font, "Sphere Battle", 80);
    titleText.setFillColor(sf::Color::White);
    titleText.setStyle(sf::Text::Bold);

    // Posizione
    sf::FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setOrigin({titleBounds.position.x + titleBounds.size.x / 2.0f, 
                         titleBounds.position.y + titleBounds.size.y / 2.0f});
    titleText.setPosition({window_width / 2.0f, (window_height / 2.0f) - 30.0f});
    
    return titleText;
}

sf::Text subtitle_text (sf::Font& font) {
    // Stile
    sf::Text subtitleText(font, "press any key", 30); // Dimensione 22px
    subtitleText.setFillColor(sf::Color(180, 180, 180)); // Grigio chiaro

    // Posizione
    sf::FloatRect subBounds = subtitleText.getLocalBounds();
    subtitleText.setOrigin({subBounds.position.x + subBounds.size.x / 2.0f, 
                            subBounds.position.y + subBounds.size.y / 2.0f});
    subtitleText.setPosition({window_width / 2.0f, (window_height / 2.0f) + 40.0f});

    return subtitleText;
}



//////////
// Loop //
//////////

int main () {
    sf::RenderWindow window (sf::VideoMode ({window_width, window_height}), window_title, sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(max_frame_rate);
    window.setMinimumSize(window.getSize());

    // Font e testi
    sf::Font font = font_loader(window);
    sf::Text titleText = title_text(font);
    sf::Text subtitleText = subtitle_text(font);


    // Loop principale
    while (window.isOpen()) {

        // Eventi
        window.handleEvents (
            [&window](const sf::Event::Closed&) { 
                handle_close(window); 
            }
        );
        
        // Display
        window.clear(sf::Color::Black);
        drawTitle(window, titleText, subtitleText);
        window.display();
    }

    return 0;
}