// Deux points �tonnant � remarquer:
// le bonhomme ne monte jamais c'est le background qui change derri�re
// il y a bien 10 et non pas 20 plats visibles en m�me temps.

// un point � remarquer est qu'il n'y a pas de temps (utilisation de clock). Le temps est calcul� � partir du framerate.
// ce qui rend la comprehension difficile d�s lors qu'il y a des deriv�es, des donn�es, pour comprendre comment elles evoluent
// est l'impossibilit� de "plotter" les donn�es en fonction du temps facilement. il doit exister des bibliotheques pour faire cela.
// un des objectifs sera peut-�tre de regarder comment immplementer cela facilement en C++. C'est la base. Il m'a sembl� voir
// que certaines personnes utilisaient un linker matplotlib en c++ , ce serait quand m�me un comble. Une alternative serait
// de regarder la bib graphique de Qt mais alors rapidement avec toutes les librairies, les dll etc, ca devient une usine a gaz.


#include <SFML/Graphics.hpp>

//#include <SFML/Vector2.hpp>
#include <time.h>
using namespace sf;

// on definit une structure qui contient deux entier, x et y.


struct point
{
	int x, y;
};

int main()
{
	srand(time(0));

	// cr�e un objet RenderWindow qui il me semble est celui de choix pour la SFML


	RenderWindow app(VideoMode(400, 533), "Doodle Game!");

	// limitation du framerate
	app.setFramerateLimit(60);

	// j'ajoute des deux lignes pour debugger facilement avec VS � droite et la fen�tre � gauche

	sf::Vector2i Position(10, 10);
	app.setPosition(Position);


	Texture t1, t2, t3;
	t1.loadFromFile("images/background.png");
	t2.loadFromFile("images/platform.png");
	t3.loadFromFile("images/doodle.png");

	Sprite sBackground(t1), sPlat(t2), sPers(t3);

	// avan c'�tait 20 mais que 10 sont utilis�s (on peut r�gler si  on veut moins de platforme)
	point plat[10];


	// on choisit al�atoirement les coordonn�es des plats entre 0 et 400 et 0 et 533.

	for (int i = 0; i < 10; i++)
	{
		plat[i].x = rand() % 400;
		plat[i].y = rand() % 533;
	}

	// variables probablement utiles pour le saut.
	int x = 100, y = 100, h = 200;
	float dx = 0, dy = 0;

	while (app.isOpen())
	{
		Event e;
		while (app.pollEvent(e))
		{
			if (e.type == Event::Closed)
				app.close();
		}

		// j'ai remarqu� qu'il n'est pas n�cessaire d'inclure ces pseudo evenement dans l
		// la gestion des evenement.
		// le bonhomme demarre � x= 100 et h=100. quand on appuie sur la fleche droite ou gauche
		// on deplace le bonhomme de + ou - 3

		if (Keyboard::isKeyPressed(Keyboard::Right)) x += 3;
		if (Keyboard::isKeyPressed(Keyboard::Left)) x -= 3;

		// il y a 20 lignes un peu complexes qui font tout!

		// il faut comprendre que il y a 60 frames /secondes. donc dy, en 1 seconde devient 30 et grossit
		// au fur et � mesure ce qui donne le sentiment de vitesse qui augmente ou diminue.


		// il est clair que quand dy=0, le bonhomme ne monte ni descend.
		// dy positif: le bonhomme descend
		// dy n�gatif, le bonhomme monte (coherent avec le zero, zero en h
		// haut � gauche.
		// dy oscille entre -10 et +10.

		dy += 0.2;

		// la position est ajust� en fonction de la valeur de dy. ce qui fait que y augmente.
		y += dy;

		// Mais si y d�passe 500 (arrive en bas sans plateforme, on set dy � -10 pour que le bonhomme remonte)
		if (y > 500)
		{
			dy = -10;
		}


// a la base, le if ne contenait pas d'accolade avant la boucle for et pourtant ca marchait. 
// peut-�tre parce que la boucle for est consider� comme une ligne d'instruction.
// neanmoins, c'est pas clair et j'ai rajout� les accolades.

// ce bloc de ligne sert � changer le y de tout les bloc (l'augmenter pour que les bloc descendent)
// quand le y du bonhomme d�passe une certaine hauteur. si un plat depasse 533 (le bas) alors on
// on cr�e un nouveau plat en haut (y=0) avec un x aleatoire.

		if (y < h) {

			for (int i = 0; i < 10; i++)
			{
				y = h;
				plat[i].y = plat[i].y - dy;
				if (plat[i].y > 533) { plat[i].y = 0; plat[i].x = rand() % 400; }
			}

		}
		// Je commence � piger:
	   // il y a deux cas : soit on arrive  � +500 (le bas) et alors il faut remonter (set  �-10)
	 // soit le bonhomme tombe sur une platforme et alors il faut remonter: set  � -10!

// si notre bonhomme s'arr�te sur un plat, alors il faut reinitiliser dy  � -10
// ce qui permet au bonhomme de rebondir (grosse vitesse de demarrage).
		for (int i = 0; i < 10; i++)
			if ((x + 50 > plat[i].x) && (x + 20 < plat[i].x + 68)
				&& (y + 70 > plat[i].y) && (y + 70 < plat[i].y + 14) && (dy > 0))  dy = -10;

		sPers.setPosition(x, y);

		// le draw(sBackground) semble remplacer le clear qui pr�cede la cr�ation d'une nouvelle frame


		app.draw(sBackground);
		app.draw(sPers);

		// toujours cette question pourquoi 10 et pas 20?
		for (int i = 0; i < 10; i++)
		{
			sPlat.setPosition(plat[i].x, plat[i].y);
			app.draw(sPlat);
		}

		app.display();
	}

	return 0;
}
