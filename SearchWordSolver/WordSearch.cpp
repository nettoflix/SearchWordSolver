#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <iostream>
#include <string>
#include <fstream>
#include<ostream>
#include <sstream>
class Example : public olc::PixelGameEngine
{
public:
	Example()
	{
		sAppName = "CACAAPALAVRAAAAAA";
	}

private:
	struct Rectangle {
		int x1 = 0;
		int y1 = 0;
		int x2 = 0;
		int y2 = 0;
	};

	struct Letter {
		char cLetter;
		Letter* neighborNorte = nullptr;
		Letter* neighborNordeste = nullptr;
		Letter* neighborLeste = nullptr;
		Letter* neighborSudeste = nullptr;
		Letter* neighborSul = nullptr;
		Letter* neighborSudoeste = nullptr;
		Letter* neighborOeste = nullptr;
		Letter* neighborNoroeste = nullptr;
		Letter* getLetterAtDirection(int direction)
		{
			if (direction == NORTE) return neighborNorte;
			if (direction == NORDESTE) return neighborNordeste;
			if (direction == LESTE) return neighborLeste;
			if (direction == SUDESTE) return neighborSudeste;
			if (direction == SUL) return neighborSul;
			if (direction == SUDOESTE) return neighborSudoeste;
			if (direction == OESTE) return neighborOeste;
			if (direction == NOROESTE) return neighborNoroeste;
		}
	};
	enum direction { NORTE = 1, NORDESTE, LESTE, SUDESTE, SUL, SUDOESTE, OESTE, NOROESTE };
	struct palavraEncontrada
	{
		std::string str;
		int x = 0;
		int y = 0;
		direction dir = NORTE;
	};
	olc::Sprite* alfabeto1;
	olc::Sprite* alfabetoAlternativo;
	std::vector<Rectangle> wordsRect;
	std::vector<std::vector<Letter>*> dimensions;
	std::vector<std::string> words;
	std::vector<palavraEncontrada> pEs;
	olc::vf2d gScale = { 1.5f,1.5f };

public:

	void drawLetter(olc::vi2d xy, char letter, olc::Sprite* alfabeto, uint32_t scale = 1)
	{
		int pos = letter;
		int col;
		if (letter == ' ')
		{
			return;
		}
		//first row of the sheet
		if (letter >= 65 && letter <= 72) //keycode from A to H
		{
			col = pos - 65;
			DrawPartialSprite(xy, alfabeto, { 8 * col,0 }, { 8,8 }, scale);
		}
		////second
		else if (letter >= 73 && letter <= 80) //keycode from I to P
		{
			col = pos - 73;
			DrawPartialSprite(xy, alfabeto, { 8 * col,8 }, { 8,8 }, scale);
		}
		////third
		else if (letter >= 81 && letter <= 88) //keycode from Q to X
		{
			col = pos - 81;
			DrawPartialSprite(xy, alfabeto, { 8 * col,16 }, { 8,8 }, scale);
		}
		else if (letter == 89 || letter == 90)
		{
			col = pos - 89;
			DrawPartialSprite(xy, alfabeto, { 8 * col,24 }, { 8,8 }, scale);
		}
		//numbers
		if (letter >= 48 && letter <= 57)
		{
			col = pos - 48;
			int start = 2; //where the numbers begin in this row
			if (col <= 5)
			{
				DrawPartialSprite(xy, alfabeto, { 8 * (col + start),24 }, { 8,8 }, scale);
			}
			else
			{
				col = pos - 54;
				DrawPartialSprite(xy, alfabeto, { (8 * col),32 }, { 8,8 }, scale);
			}
		}
	}
	void drawString(olc::vi2d xy, std::string word, olc::Sprite* alfabeto, uint32_t scale)
	{

		for (int i = 0; i < word.size(); ++i)
		{
			char letter = word.at(i);
			drawLetter(xy, letter, alfabeto, scale);
			xy.x += scale * gScale.x;
		}

	}
	void setPuzzleDimensions(std::ifstream& inFile)
	{
		std::string str;
		int y = 0;
		while (std::getline(inFile, str))
		{
			dimensions.push_back(new std::vector<Letter>);
			for (int x = 0; x < str.size(); x++)
			{
				char currentlyChar = str.at(x);
				if (currentlyChar != ' ')
				{
					Letter* currentLetter = new Letter();
					currentLetter->cLetter = currentlyChar;
					dimensions.at(y)->push_back(*currentLetter);
				}
			}
			y++;
		}
		//set neighbors
		for (int r = 0; r < dimensions.size(); ++r)
		{
			for (int c = 0; c < dimensions.at(r)->size(); ++c)
			{
				Letter& currentLetter = dimensions.at(r)->at(c);
				if (r > 0)													currentLetter.neighborNorte = &dimensions.at(r - 1)->at(c);
				if (r > 0 && c < dimensions.at(r)->size() - 1)					currentLetter.neighborNordeste = &dimensions.at(r - 1)->at(c + 1);
				if (c < dimensions.at(r)->size() - 1)						currentLetter.neighborLeste = &dimensions.at(r)->at(c + 1);
				if (r < dimensions.size() - 1 && c < dimensions.at(r)->size() - 1)	currentLetter.neighborSudeste = &dimensions.at(r + 1)->at(c + 1);
				if (r < dimensions.size() - 1)								currentLetter.neighborSul = &dimensions.at(r + 1)->at(c);
				if (r < dimensions.size() - 1 && c >0)						currentLetter.neighborSudoeste = &dimensions.at(r + 1)->at(c - 1);
				if (c > 0)												currentLetter.neighborOeste = &dimensions.at(r)->at(c - 1);
				if (r > 0 && c > 0)										currentLetter.neighborNoroeste = &dimensions.at(r - 1)->at(c - 1);

			}
		}



	}
	void initializeWordsFromFile(std::string fileName)
	{

		std::ifstream inFile = std::ifstream(fileName);
		std::string wholeText((std::istreambuf_iterator<char>(inFile)), (std::istreambuf_iterator<char>())); //store the all the text content from the file into the string
		std::stringstream ss;
		ss.str(wholeText);
		std::string tss = ss.str();
		while (ss.good()) //read the string until the end of it
		{
			std::string substr;
			std::getline(ss, substr, ','); //find the word in ss and stores it in substr, the word ends before the ',' character. ss then throws the word and the "," found away
			words.push_back(substr); // the word found is put in the words vector

		}
		//the while loop fails because the ss is empty, all words were recorded.
		for (auto& word : words)
		{
			std::cout << word << "\n";
		}

	}
	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
		alfabeto1 = new olc::Sprite("Imagens/font2.png");
		alfabetoAlternativo = new olc::Sprite("Imagens/font2alternativo.png");
		std::ifstream inFile = std::ifstream("puzzle.txt");

		setPuzzleDimensions(inFile);
		initializeWordsFromFile("palavras.txt");
		//words = {"CADEIAALIMENTAR", "CARNIVORO", "COMUNIDADE", "ECOLOGIA", "ECOSSISTEMA", "ESPECIE", "FITOPLANCTON", "HERBIVORO", "ONIVORO", "PIRAMIDE", "POPULACAO", "TEIAALIMENTAR", "ZOOPLANCTON"};
		std::string wordToPrint = "";
		for (auto& word : words)
		{
			bool wordFound = false;
			char firstLetter = word.at(0);
			char secondLetter = word.at(1);
			int direction = 0;
			for (int r = 0; r < dimensions.size(); ++r)
			{
				for (int c = 0; c < dimensions.at(r)->size(); ++c)
				{
					Letter* currentLetter = &dimensions.at(r)->at(c);
					char currentChar = currentLetter->cLetter;
					if (currentChar == firstLetter) //looking for first letter of current the word
					{
						std::string wordStack;
						wordStack += firstLetter;
						//check the neighbors looking for the second letter of the current word
						//char charNorte' '; char charNordeste' '; 
						char charNorte = ' '; char charNordeste = ' '; char charLeste = ' '; char charSudeste = ' '; char charSul = ' '; char charSudoeste = ' '; char charOeste = ' '; char charNoroeste = ' ';
						if (currentLetter->neighborNorte != nullptr)  charNorte = currentLetter->neighborNorte->cLetter;
						if (currentLetter->neighborNordeste != nullptr) charNordeste = currentLetter->neighborNordeste->cLetter;
						if (currentLetter->neighborLeste != nullptr)charLeste = currentLetter->neighborLeste->cLetter;
						if (currentLetter->neighborSudeste != nullptr) charSudeste = currentLetter->neighborSudeste->cLetter;
						if (currentLetter->neighborSul != nullptr) charSul = currentLetter->neighborSul->cLetter;
						if (currentLetter->neighborSudoeste != nullptr) charSudoeste = currentLetter->neighborSudoeste->cLetter;
						if (currentLetter->neighborOeste != nullptr) charOeste = currentLetter->neighborOeste->cLetter;
						if (currentLetter->neighborNoroeste != nullptr) charNoroeste = currentLetter->neighborNoroeste->cLetter;


						std::vector<int> directions;
						if (secondLetter == charNorte)
						{
							direction = NORTE;
							directions.push_back(direction);
						}
						if (secondLetter == charNordeste)
						{
							direction = NORDESTE;
							directions.push_back(direction);
						}
						if (secondLetter == charLeste)
						{
							direction = LESTE;
							directions.push_back(direction);
						}
						if (secondLetter == charSudeste)
						{
							direction = SUDESTE;
							directions.push_back(direction);
						}
						if (secondLetter == charSul)
						{
							direction = SUL;
							directions.push_back(direction);
						}
						if (secondLetter == charSudoeste)
						{
							direction = SUDOESTE;
							directions.push_back(direction);
						}
						if (secondLetter == charOeste)
						{
							direction = OESTE;
							directions.push_back(direction);
						}
						if (secondLetter == charNoroeste)
						{
							direction = NOROESTE;
							directions.push_back(direction);
						}


						for (int i = 0; i < directions.size(); i++) //if the second letter was found, keep searching in this direction
						{
							int currentDirection = directions.at(i);
							Rectangle wRect;
							palavraEncontrada pE;
							wRect.x1 = c * (gScale.x * 8);
							wRect.y1 = r * (gScale.x * 8);
							int nextCharPosition = 1;
							if (currentLetter != nullptr) currentLetter = currentLetter->getLetterAtDirection(currentDirection);
							while (currentLetter != nullptr)
							{

								if (nextCharPosition >= word.size()) break;
								if (currentLetter->cLetter == word.at(nextCharPosition))
								{
									currentChar = currentLetter->cLetter;
									wordStack += currentChar;
									if (currentLetter->getLetterAtDirection(currentDirection) != nullptr) currentLetter = currentLetter->getLetterAtDirection(currentDirection);
									nextCharPosition++;
								}
								else
								{
									break;
								}
							}
							//std::cout << wordStack << "\n";
							if (wordStack == word) //the word was found, so save the cordinates to draw and mark it
							{
								pE.str = wordStack;
								pE.x = wRect.x1;
								pE.y = wRect.y1;

								if (currentDirection == NORTE)
								{
									pE.dir = NORTE;
									/*wRect.x1 += (gScale.x*8)/2;
									wRect.y1 += (gScale.x * 8);
									wRect.x2 = wRect.x1;
									wRect.y2 = wRect.y1 -((gScale.x * 8) *word.size());*/
								}
								else if (currentDirection == NORDESTE)
								{
									pE.dir = NORDESTE;
									/*	wRect.y1 += (gScale.x * 8);
										wRect.x2 = wRect.x1 + (gScale.x * 8) *word.size();
										wRect.y2 = wRect.y1 - ((gScale.x * 8) * (word.size()));*/
								}
								else if (currentDirection == NOROESTE)
								{
									pE.dir = NOROESTE;
									/*	wRect.x1 += (gScale.x * 8);
										wRect.y1 += (gScale.x * 8);
										wRect.x2 = wRect.x1 -  ((gScale.x * 8) *word.size());
										wRect.y2 = wRect.y1 - ((gScale.x * 8) * word.size());*/
								}
								else if (currentDirection == SUL)
								{
									pE.dir = SUL;
									/*wRect.x2 = (gScale.x * 8);
									wRect.y2 = (gScale.x * 8) * (word.size());*/
								}
								else if (currentDirection == SUDESTE)
								{
									pE.dir = SUDESTE;
									/*wRect.x2 = wRect.x1+ (gScale.x * 8) *(word.size());
									wRect.y2 = wRect.y1 + (gScale.x * 8) * word.size();*/
								}
								else if (currentDirection == SUDOESTE)
								{
									pE.dir = SUDOESTE;
									/*	wRect.x1 += (gScale.x * 8);
										wRect.x2 = wRect.x1 - ((gScale.x * 8) * (word.size()));
										wRect.y2 = wRect.y1 + (gScale.x * 8) * (word.size());*/
								}
								else if (currentDirection == LESTE)
								{
									pE.dir = LESTE;
									/*wRect.y1 += (gScale.x * 8) /2;
									wRect.x2 = wRect.x1 + ((gScale.x * 8) * (word.size()));
									wRect.y2 += wRect.y1*/;
								}
								else if (currentDirection == OESTE)
								{
									pE.dir = OESTE;
									/*	wRect.y1 += (gScale.x * 8) /2;
										wRect.x1 += (gScale.x * 8);
										wRect.x2 = wRect.x1 -((gScale.x * 8) *(word.size()));
										wRect.y2 = wRect.y1;*/
								}
								pEs.push_back(pE);
								wordsRect.push_back(wRect);
								wordFound;
								goto end;
							}
							else
							{
								// we didn't find the word in the first direction, so reset the word that we were constructing and the letter too
								wordStack = word.at(0);
								currentLetter = &dimensions.at(r)->at(c);
							}

						}

					}
				}
				if (wordFound) end: break;
			}

		}

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{		// called once per frame
		//for (int x = 0; x < ScreenWidth(); x++)
		//	for (int y = 0; y < ScreenHeight(); y++)
		//		Draw(x, y, olc::Pixel(rand() % 255, rand() % 255, rand()% 255));
		FillRect({ 0,0 }, { ScreenWidth(), ScreenHeight() });
		//drawString({ 50,50 },"ZZZA", 2);
		//drawLetter({ 50,100 }, 'YZ');
		int x = 0;
		int y = 0;

		for (int r = 0; r < dimensions.size(); ++r)
		{
			for (int c = 0; c < dimensions.at(r)->size(); ++c)
			{
				drawLetter({ x,y }, dimensions.at(r)->at(c).cLetter, alfabeto1, gScale.x);
				x += (gScale.x * 8);
			}
			x = 0;
			y += gScale.x * 8;
		}
		//DrawRect({ wordRect.x, wordRect.y }, { wordRect.width, wordRect.height }, olc::DARK_YELLOW);
		for (auto& pe : pEs)
		{
			drawLetter({ pe.x,pe.y }, pe.str.at(0), alfabetoAlternativo, gScale.x); //draw the first letter in the pe.position

			//for the second letter and the other ones you need to check the direction

			//if norte
			if (pe.dir == NORTE)
			{
				for (int i = 1; i < pe.str.size(); i++)
				{
					drawLetter({ pe.x , static_cast<int>((-i * 8 * gScale.x) + pe.y) }, pe.str.at(i), alfabetoAlternativo, gScale.x);

				}
			}
			if (pe.dir == NORDESTE)
			{
				for (int i = 1; i < pe.str.size(); i++)
				{
					drawLetter({ static_cast<int>(pe.x + (i * 8 * gScale.x)) , static_cast<int>((-i * 8 * gScale.x) + pe.y) }, pe.str.at(i), alfabetoAlternativo, gScale.x);

				}
			}
			if (pe.dir == NOROESTE)
			{
				for (int i = 1; i < pe.str.size(); i++)
				{
					drawLetter({ static_cast<int>(pe.x + (-i * 8 * gScale.x)) , static_cast<int>((-i * 8 * gScale.x) + pe.y) }, pe.str.at(i), alfabetoAlternativo, gScale.x);

				}
			}
			if (pe.dir == OESTE)
			{
				for (int i = 1; i < pe.str.size(); i++)
				{
					drawLetter({ static_cast<int>(pe.x + (-i * 8 * gScale.x)) , pe.y }, pe.str.at(i), alfabetoAlternativo, gScale.x);

				}
			}
			if (pe.dir == LESTE)
			{
				for (int i = 1; i < pe.str.size(); i++)
				{
					drawLetter({ static_cast<int>(pe.x + (i * 8 * gScale.x)) , pe.y }, pe.str.at(i), alfabetoAlternativo, gScale.x);

				}
			}
			if (pe.dir == SUL)
			{
				for (int i = 1; i < pe.str.size(); i++)
				{
					drawLetter({ pe.x , static_cast<int>(pe.y + (i * 8 * gScale.x)) }, pe.str.at(i), alfabetoAlternativo, gScale.x);

				}
			}
			if (pe.dir == SUDESTE)
			{
				for (int i = 1; i < pe.str.size(); i++)
				{
					drawLetter({ static_cast<int>(pe.x + (i * 8 * gScale.x)) , static_cast<int>(pe.y + (i * 8 * gScale.x)) }, pe.str.at(i), alfabetoAlternativo, gScale.x);

				}
			}
			if (pe.dir == SUDOESTE)
			{
				for (int i = 1; i < pe.str.size(); i++)
				{
					drawLetter({ static_cast<int>(pe.x + (-i * 8 * gScale.x)) , static_cast<int>(pe.y + (i * 8 * gScale.x)) }, pe.str.at(i), alfabetoAlternativo, gScale.x);

				}
			}
		}
		/*	for (auto& Rect : wordsRect)
			{
				DrawLine({ Rect.x1,Rect.y1 }, { Rect.x2, Rect.y2 }, olc::RED);
			}*/
			//DrawLine({ 20,20 }, {20, 80}, olc::DARK_YELLOW);
		return true;
	}
};


int main()
{
	Example demo;
	if (demo.Construct(400, 240, 4, 4))
		demo.Start();

	return 0;
}
