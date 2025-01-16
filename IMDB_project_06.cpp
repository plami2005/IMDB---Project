#include <iostream>
#include <fstream>
//#include <cstring>

const int MAX_FILMS = 200;
const int MAX_TITLE_LENGTH = 50;
const int MAX_GENRE_LENGTH = 20;
const int MAX_NAME_LENGTH = 50;
const int MAX_ACTORS = 3;

struct Film {
    
    char title[MAX_TITLE_LENGTH];
    int year;
    char genre[MAX_GENRE_LENGTH];
    char director[MAX_NAME_LENGTH];
    char actors[MAX_ACTORS][MAX_NAME_LENGTH];
    float rating;
    int ratingCount;

    Film() : year(0), rating(0), ratingCount(0) {
        memset(title, 0, MAX_TITLE_LENGTH);
        memset(genre, 0, MAX_GENRE_LENGTH);
        memset(director, 0, MAX_NAME_LENGTH);
        for (int i = 0; i < MAX_ACTORS; ++i) {
            memset(actors[i], 0, MAX_NAME_LENGTH);
        }
    }
};

class IMDBSystem {
private:
    Film films[MAX_FILMS];
    int filmCount;
public:
    IMDBSystem() : filmCount(0) {}

    void loadFilmsFromFile(const char* fileName) {
        std::ifstream file(fileName);
        if (!file) {
            std::cout << "Failed to open file: " << fileName << std::endl;
            return;
        }
        char line[256];

        while (file.getline(line, 256) && filmCount < MAX_FILMS) {
            Film newFilm;
            char* next_token = nullptr;
            char* token = strtok_s(line, ",", &next_token);
            strncpy_s(newFilm.title, token, MAX_TITLE_LENGTH - 1);

            token = strtok_s(nullptr, ",", &next_token);
            newFilm.year = atoi(token);

            token = strtok_s(nullptr, ",", &next_token);
            strncpy_s(newFilm.genre, token, MAX_GENRE_LENGTH - 1);

            token = strtok_s(nullptr, ",", &next_token);
            strncpy_s(newFilm.director, token, MAX_NAME_LENGTH - 1);

            for (int i = 0; i < MAX_ACTORS; ++i) {
                token = strtok_s(nullptr, ",", &next_token);
                if (token) {
                    strncpy_s(newFilm.actors[i], token, MAX_NAME_LENGTH - 1);
                }
            }
            films[filmCount++] = newFilm;
        }
        file.close();
    }

}

int main() {
    IMDBSystem imdb;
    imdb.loadFilmsFromFile("films.txt"); // Зареждане на филмите от файла

    int userType;
    std::cout << "Choose type user:\n1. Administrator\n2. Normal user\nYour choise: ";
    std::cin >> userType;
    //администратор
    if (userType == 1) {
        int choice;
        do {
            std::cout << "\nAdmin Menu:" << std::endl;
            std::cout << "1. Add new film" << std::endl;
            std::cout << "2. Delete a film" << std::endl;
            std::cout << "3. Search movie by ganre" << std::endl;
            std::cout << "4. Search movie by title" << std::endl;
            std::cout << "5. List all films" << std::endl;
            std::cout << "6. Change movie info" << std::endl;
            std::cout << "7. Sort films (by title or rating)" << std::endl;
            std::cout << "8. Exit" << std::endl;
            std::cout << "Enter your choice: ";
            std::cin >> choice;

            switch (choice) {
            case 1:
                //imdb.addFilm();
                break;
            case 2:
                //imdb.deleteFilm();
                break;
            case 3:
                //imdb.searchByGenre();
                break;
            case 4:
                //imdb.searchByTitle();
                break;
            case 5:
                //imdb.listFilms();
                break;
            case 6:
                //imdb.editFilm();
                break;
            case 7:
                //imdb.sortFilms();
                break;
            case 8:
                std::cout << "Exiting admin menu..." << std::endl;
                break;
            default:
                std::cout << "Invalid choice. Please try again!" << std::endl;
            }
        } while (choice != 8);
    }
    //потребител
    if (userType == 2) { // Меню за обикновен потребител
        int choice;
        do {
            std::cout << "\nUser menu:" << std::endl;
            std::cout << "1. List of movies" << std::endl;
            std::cout << "2. Rate movie" << std::endl;
            std::cout << "3. Search movie by ganre" << std::endl;
            std::cout << "4. Search movie by title" << std::endl;
            std::cout << "5. Sort movies" << std::endl;
            std::cout << "6. Filter movies by rating" << std::endl;
            std::cout << "7. Exit" << std::endl;
            std::cout << "Choose an option: ";
            std::cin >> choice;
            switch (choice) {
            case 1:
                //imdb.listFilms();
                break;
            case 2:
                //imdb.rateFilm();
                break;
            case 3:
                //imdb.searchByGenre();
                break;
            case 4:
                //imdb.searchByTitle();
                break;
            case 5:
                //imdb.sortFilms();
                break;
            case 6:
                //imdb.filterByRating();
                break;
            case 7:
                std::cout << "Exiting system..." << std::endl;
                break;
            default:
                std::cout << "Invalid choise. Try again!" << std::endl;
            }
        } while (choice != 7);
    }
    else {
        std::cout << "Invalid choise!" << std::endl;
    }

    return 0;

}