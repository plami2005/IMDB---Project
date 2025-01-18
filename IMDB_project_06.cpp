#include <iostream>
#include <fstream>
#include <cstring> // За работа с char масиви


const int MAX_FILMS = 200;
const int MAX_TITLE_LENGTH = 50;
const int MAX_GENRE_LENGTH = 20;
const int MAX_NAME_LENGTH = 50;
const int MAX_ACTORS = 3;
//обект с име филм
struct Film {
    //променливи на филм
    char title[MAX_TITLE_LENGTH];
    int year;
    char genre[MAX_GENRE_LENGTH];
    char director[MAX_NAME_LENGTH];
    char actors[MAX_ACTORS][MAX_NAME_LENGTH];
    float rating;
    int ratingCount;
    //конструктор
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

    void sortByTitle() {
        for (int i = 0; i < filmCount - 1; ++i) {
            for (int j = 0; j < filmCount - i - 1; ++j) {
                // Сравняваме филмите по заглавие (лексикографски)
                if (std::strcmp(films[j].title, films[j + 1].title) > 0) {
                    // Разменяме местата на филмите
                    Film temp = films[j];
                    films[j] = films[j + 1];
                    films[j + 1] = temp;
                }
            }
        }
        std::cout << "Movies sucsesfully sorted by title!" << std::endl;
    }

    void sortByRating() {
        for (int i = 0; i < filmCount - 1; ++i) {
            for (int j = 0; j < filmCount - i - 1; ++j) {
                // Изчисляваме средния рейтинг за текущите два филма
                float ratingA = (films[j].ratingCount > 0 ? films[j].rating / films[j].ratingCount : 0);
                float ratingB = (films[j + 1].ratingCount > 0 ? films[j + 1].rating / films[j + 1].ratingCount : 0);

                // Ако рейтингът на текущия филм е по-нисък от този на следващия, разменяме местата им
                if (ratingA < ratingB) {
                    Film temp = films[j];
                    films[j] = films[j + 1];
                    films[j + 1] = temp;
                }
            }
        }
        std::cout << "Movies successfully sorted by rating!" << std::endl;
    }

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

    void editFilm() {
        if (filmCount == 0) {
            std::cout << "No films available to edit." << std::endl;
            return;
        }

        listFilms();
        int choice;
        std::cout << "Enter the number of the film to edit: ";
        std::cin >> choice;

        if (choice < 1 || choice > filmCount) {
            std::cout << "Invalid choice!" << std::endl;
            return;
        }

        Film& selectedFilm = films[choice - 1]; // Избира избрания филм

        int editChoice;
        do {
            std::cout << "\nEditing Film: " << selectedFilm.title << std::endl;
            std::cout << "1. Edit Title" << std::endl;
            std::cout << "2. Edit Year" << std::endl;
            std::cout << "3. Edit Genre" << std::endl;
            std::cout << "4. Edit Director" << std::endl;
            std::cout << "5. Edit Actors" << std::endl;
            std::cout << "6. Edit Rating" << std::endl;
            std::cout << "7. Exit Editing" << std::endl;
            std::cout << "Enter your choice: ";
            std::cin >> editChoice;

            std::cin.ignore(); // Изчиства буфера за въвеждане

            switch (editChoice) {
            case 1:
                std::cout << "Enter new title: ";
                std::cin.getline(selectedFilm.title, MAX_TITLE_LENGTH);
                std::cout << "Title updated successfully!" << std::endl;
                break;
            case 2:
                std::cout << "Enter new year: ";
                std::cin >> selectedFilm.year;
                std::cout << "Year updated successfully!" << std::endl;
                break;
            case 3:
                std::cout << "Enter new genre: ";
                std::cin.ignore();
                std::cin.getline(selectedFilm.genre, MAX_GENRE_LENGTH);
                std::cout << "Genre updated successfully!" << std::endl;
                break;
            case 4:
                std::cout << "Enter new director: ";
                std::cin.getline(selectedFilm.director, MAX_NAME_LENGTH);
                std::cout << "Director updated successfully!" << std::endl;
                break;
            case 5:
                for (int i = 0; i < MAX_ACTORS; ++i) {
                    std::cout << "Enter new actor " << (i + 1) << ": ";
                    std::cin.getline(selectedFilm.actors[i], MAX_NAME_LENGTH);
                }
                std::cout << "Actors updated successfully!" << std::endl;
                break;
            case 6:
                std::cout << "Enter new rating (total score): ";
                std::cin >> selectedFilm.rating;
                std::cout << "Enter new rating count: ";
                std::cin >> selectedFilm.ratingCount;
                std::cout << "Rating updated successfully!" << std::endl;
                break;
            case 7:
                std::cout << "Exiting editing menu..." << std::endl;
                break;
            default:
                std::cout << "Invalid choice! Please try again." << std::endl;
            }
        } while (editChoice != 7);
    }

    void addFilm() {
        if (filmCount >= MAX_FILMS) {
            std::cout << "Database is full! Cannot add more films." << std::endl;
            return;
        }
        Film newFilm;
        std::cin.ignore();
        std::cout << "Enter film title: ";
        std::cin.getline(newFilm.title, MAX_TITLE_LENGTH);
        std::cout << "Enter release year: ";
        std::cin >> newFilm.year;
        std::cin.ignore();
        std::cout << "Enter genre: ";
        std::cin.getline(newFilm.genre, MAX_GENRE_LENGTH);
        std::cout << "Enter director: ";
        std::cin.getline(newFilm.director, MAX_NAME_LENGTH);
        for (int i = 0; i < MAX_ACTORS; ++i) {
            std::cout << "Enter actor " << (i + 1) << ": ";
            std::cin.getline(newFilm.actors[i], MAX_NAME_LENGTH);
        }
        films[filmCount++] = newFilm;
        std::ofstream file("films.txt", std::ios::app); // Режим на добавяне
        if (!file) {
            std::cerr << "Failed to open file for writing." << std::endl;
            return;
        }
        file << newFilm.title << "," << newFilm.year << "," << newFilm.genre << "," << newFilm.director;
        for (int i = 0; i < MAX_ACTORS; ++i) {
            file << "," << newFilm.actors[i];
        }
        file << std::endl;
        file.close();

        std::cout << "Film added successfully!" << std::endl;
    }

    void deleteFilm() {
        if (filmCount == 0) {
            std::cout << "No films to delete." << std::endl;
            return;
        }
        listFilms();
        int index;
        std::cout << "Enter the number of the film to delete: ";
        std::cin >> index;
        if (index < 1 || index > filmCount) {
            std::cout << "Invalid choice!" << std::endl;
            return;
        }
        for (int i = index - 1; i < filmCount - 1; ++i) {
            films[i] = films[i + 1];
        }
        --filmCount;
        std::ofstream file("films.txt", std::ios::trunc); // Отваря файла за презапис
        if (!file) {
            std::cerr << "Failed to open file for updating." << std::endl;
            return;
        }
        for (int i = 0; i < filmCount; ++i) {
            file << films[i].title << "," << films[i].year << "," << films[i].genre << "," << films[i].director;
            for (int j = 0; j < MAX_ACTORS; ++j) {
                file << "," << films[i].actors[j];
            }
            file << std::endl;
        }
        file.close();

        std::cout << "Film deleted successfully!" << std::endl;
    }




    void listFilms() const {
        if (filmCount == 0) {
            std::cout << "No movies availlable." << std::endl;
            return;
        }
        for (int i = 0; i < filmCount; ++i) {
            std::cout << i + 1 << ". " << films[i].title << " (" << films[i].year << ") - "
                << films[i].genre << " - Director: " << films[i].director
                << " - Actors: ";
            for (int j = 0; j < MAX_ACTORS; ++j) {
                if (strlen(films[i].actors[j]) > 0) {
                    std::cout << films[i].actors[j];
                    if (j < MAX_ACTORS - 1) std::cout << ", ";
                }
            }
            std::cout << " - Rating: " << (films[i].ratingCount > 0 ? films[i].rating / films[i].ratingCount : 0)
                << std::endl;
        }
    }

    void searchByGenre() const {
        if (filmCount == 0) {
            std::cout << "No movies found." << std::endl;
            return;
        }
        char genre[MAX_GENRE_LENGTH];
        std::cout << "Enter genre : ";
        std::cin.ignore();
        std::cin.getline(genre, MAX_GENRE_LENGTH);

        bool found = false;
        for (int i = 0; i < filmCount; ++i) {
            if (std::strcmp(films[i].genre, genre) == 0) {
                std::cout << i + 1 << ". " << films[i].title << " (" << films[i].year << ") - "
                    << films[i].director << std::endl;
                found = true;
            }
        }
        if (!found) {
            std::cout << "No movies with this genre : \"" << genre << "\"." << std::endl;
        }
    }

    void searchByTitle() const {
        if (filmCount == 0) {
            std::cout << "No movies found." << std::endl;
            return;
        }
        char titlePart[MAX_TITLE_LENGTH];
        std::cout << "Enter a part of the title: ";
        std::cin.ignore();
        std::cin.getline(titlePart, MAX_TITLE_LENGTH);

        bool found = false;
        for (int i = 0; i < filmCount; ++i) {
            if (std::strstr(films[i].title, titlePart) != nullptr) {
                std::cout << i + 1 << ". " << films[i].title << " (" << films[i].year << ") - "
                    << films[i].genre << " - Director: " << films[i].director << std::endl;
                found = true;
            }
        }
        if (!found) {
            std::cout << "There are no movies with this title\"" << titlePart << "\"." << std::endl;
        }
    }

    void sortFilms() {
        int choice;
        std::cout << "Sort movies:\n1. By title\n2. By rating\nYour choise: ";
        std::cin >> choice;
        if (choice == 1) {
            sortByTitle();
        }
        else if (choice == 2) {
            sortByRating();
        }
        else {
            std::cout << "Invalid choise!" << std::endl;
        }
        listFilms();
    }

    void rateFilm() {
        if (filmCount == 0) {
            std::cout << "There aren't any movies to rate." << std::endl;
            return;
        }
        listFilms();
        int choice;
        std::cout << "Choose movie to rate: (1-" << filmCount << "): ";
        std::cin >> choice;
        if (choice < 1 || choice > filmCount) {
            std::cout << "Invalid choise!" << std::endl;
            return;
        }
        float rating;
        std::cout << "Rate movie: (1-10): ";
        std::cin >> rating;
        if (rating < 1 || rating > 10) {
            std::cout << "Invalid rating!" << std::endl;
            return;
        }
        films[choice - 1].rating += rating;
        films[choice - 1].ratingCount += 1;
        float averageRating = films[choice - 1].rating / films[choice - 1].ratingCount;

        std::cout << "Thanks for rating!" << std::endl;
        std::cout << "The updated average rating for \"" << films[choice - 1].title
            << "\" is: " << averageRating << std::endl;
    }

    void filterByRating() const {
        if (filmCount == 0) {
            std::cout << "No films available." << std::endl;
            return;
        }

        float minRating;
        std::cout << "Enter minimum rating: ";
        std::cin >> minRating;

        bool found = false;
        for (int i = 0; i < filmCount; ++i) {
            float avgRating = (films[i].ratingCount > 0 ? films[i].rating / films[i].ratingCount : 0);
            if (avgRating >= minRating) {
                std::cout << i + 1 << ". " << films[i].title << " (" << films[i].year << ") - "
                    << films[i].genre << " - DIrector: " << films[i].director
                    << " - Rating: " << avgRating << std::endl;
                found = true;
            }
        }

        if (!found) {
            std::cout << "No films found with rating greater than or equal to " << minRating << "." << std::endl;
        }
    }
};

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
                imdb.addFilm();
                break;
            case 2:
                imdb.deleteFilm();
                break;
            case 3:
                imdb.searchByGenre();
                break;
            case 4:
                imdb.searchByTitle();
                break;
            case 5:
                imdb.listFilms();
                break;
            case 6:
                imdb.editFilm();
                break;
            case 7:
                imdb.sortFilms();
                break;
            case 8:
                std::cout << "Exiting admin menu..." << std::endl;
                break;
            }
        } while (choice != 8);
    }
    else
    {
        std::cout << "Invalid choice. Please try again!" << std::endl;
    }
    //потребител
    if (userType == 2) { // Меню за обикновен потребител
        int choice;
        do {
            std::cout << "\nUser menu:" << std::endl;
            std::cout << "1. List of movies" << std::endl;
            std::cout << "2. Rate movie" << std::endl;
            std::cout << "3. Search movie by genre" << std::endl;
            std::cout << "4. Search movie by title" << std::endl;
            std::cout << "5. Sort movies" << std::endl;
            std::cout << "6. Filter movies by rating" << std::endl;
            std::cout << "7. Exit" << std::endl;
            std::cout << "Choose an option: ";
            std::cin >> choice;
            switch (choice) {
            case 1:
                imdb.listFilms();
                break;
            case 2:
                imdb.rateFilm();
                break;
            case 3:
                imdb.searchByGenre();
                break;
            case 4:
                imdb.searchByTitle();
                break;
            case 5:
                imdb.sortFilms();
                break;
            case 6:
                imdb.filterByRating();
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
