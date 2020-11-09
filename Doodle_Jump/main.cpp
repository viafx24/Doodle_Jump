// Deux points étonnant à remarquer:
// le bonhomme ne monte jamais c'est le background qui change derrière
// il y a bien 10 et non pas 20 plats visibles en même temps.

// un point à remarquer est qu'il n'y a pas de temps (utilisation de clock). Le temps est calculé à partir du framerate.
// ce qui rend la comprehension difficile dés lors qu'il y a des derivées, des données, pour comprendre comment elles evoluent
// est l'impossibilité de "plotter" les données en fonction du temps facilement. il doit exister des bibliotheques pour faire cela.
// un des objectifs sera peut-être de regarder comment immplementer cela facilement en C++. C'est la base. Il m'a semblé voir
// que certaines personnes utilisaient un linker matplotlib en c++ , ce serait quand même un comble. Une alternative serait
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

	// crée un objet RenderWindow qui il me semble est celui de choix pour la SFML


	RenderWindow app(VideoMode(400, 533), "Doodle Game!");

	// limitation du framerate
	app.setFramerateLimit(60);

	// j'ajoute des deux lignes pour debugger facilement avec VS à droite et la fenètre à gauche

	sf::Vector2i Position(10, 10);
	app.setPosition(Position);


	Texture t1, t2, t3;
	t1.loadFromFile("images/background.png");
	t2.loadFromFile("images/platform.png");
	t3.loadFromFile("images/doodle.png");

	Sprite sBackground(t1), sPlat(t2), sPers(t3);

	// avan c'était 20 mais que 10 sont utilisés (on peut régler si  on veut moins de platforme)
	point plat[10];


	// on choisit aléatoirement les coordonnées des plats entre 0 et 400 et 0 et 533.

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

		// j'ai remarqué qu'il n'est pas nécessaire d'inclure ces pseudo evenement dans l
		// la gestion des evenement.
		// le bonhomme demarre à x= 100 et h=100. quand on appuie sur la fleche droite ou gauche
		// on deplace le bonhomme de + ou - 3

		if (Keyboard::isKeyPressed(Keyboard::Right)) x += 3;
		if (Keyboard::isKeyPressed(Keyboard::Left)) x -= 3;

		// il y a 20 lignes un peu complexes qui font tout!

		// il faut comprendre que il y a 60 frames /secondes. donc dy, en 1 seconde devient 30 et grossit
		// au fur et à mesure ce qui donne le sentiment de vitesse qui augmente ou diminue.


		// il est clair que quand dy=0, le bonhomme ne monte ni descend.
		// dy positif: le bonhomme descend
		// dy négatif, le bonhomme monte (coherent avec le zero, zero en h
		// haut à gauche.
		// dy oscille entre -10 et +10.

		dy += 0.2;

		// la position est ajusté en fonction de la valeur de dy. ce qui fait que y augmente.
		y += dy;

		// Mais si y dépasse 500 (arrive en bas sans plateforme, on set dy à -10 pour que le bonhomme remonte)
		if (y > 500)
		{
			dy = -10;
		}


// a la base, le if ne contenait pas d'accolade avant la boucle for et pourtant ca marchait. 
// peut-être parce que la boucle for est consideré comme une ligne d'instruction.
// neanmoins, c'est pas clair et j'ai rajouté les accolades.

// ce bloc de ligne sert à changer le y de tout les bloc (l'augmenter pour que les bloc descendent)
// quand le y du bonhomme dépasse une certaine hauteur. si un plat depasse 533 (le bas) alors on
// on crée un nouveau plat en haut (y=0) avec un x aleatoire.

		if (y < h) {

			for (int i = 0; i < 10; i++)
			{
				y = h;
				plat[i].y = plat[i].y - dy;
				if (plat[i].y > 533) { plat[i].y = 0; plat[i].x = rand() % 400; }
			}

		}
		// Je commence à piger:
	   // il y a deux cas : soit on arrive  à +500 (le bas) et alors il faut remonter (set  à-10)
	 // soit le bonhomme tombe sur une platforme et alors il faut remonter: set  à -10!

// si notre bonhomme s'arrête sur un plat, alors il faut reinitiliser dy  à -10
// ce qui permet au bonhomme de rebondir (grosse vitesse de demarrage).
		for (int i = 0; i < 10; i++)
			if ((x + 50 > plat[i].x) && (x + 20 < plat[i].x + 68)
				&& (y + 70 > plat[i].y) && (y + 70 < plat[i].y + 14) && (dy > 0))  dy = -10;

		sPers.setPosition(x, y);

		// le draw(sBackground) semble remplacer le clear qui précede la création d'une nouvelle frame


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
