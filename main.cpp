#include <iostream>
#include <SFML/Graphics.hpp>
#define SPEED 600

// C++ program to generate random numbers
#include <cstdlib>
#include <time.h>

int  w = 500,h = 360;

int randomNumber(int range){
    srand(time(0));

    return rand()%range;
}


using namespace sf;

struct Direction{
    bool xIn = true,yIn = false, xDe = false, yDe = false;

    void xIn_true(){
        xIn = true,yIn = false, xDe = false, yDe = false;
    }

    void yIn_true(){
        xIn = false,yIn = true, xDe = false, yDe = false;
    }

    void xDe_true(){
        xIn = false,yIn = false, xDe = true, yDe = false;
    }

    void yDe_true(){
        xIn = false,yIn = false, xDe = false, yDe = true;
    }
};

void direction(Direction *d){

    if (Keyboard::isKeyPressed(Keyboard::Down)  && (!d->yIn)){
        d->yIn_true();
    }else if(Keyboard::isKeyPressed(Keyboard::Up)  && (!d->yIn)){
        d->yDe_true();
    }else if(Keyboard::isKeyPressed(Keyboard::Right)  && (!d->xDe)){
        d->xIn_true();
    }else if(Keyboard::isKeyPressed(Keyboard::Left)  && (!d->xIn)){
        d->xDe_true();
    }

}

class Food: public RectangleShape{
public:
    int x;
    int y;
    void setPos(){
        Food::setPosition(x,y);
    }
    Food(int xp, int yp){
        Food::setSize(Vector2f(10,10));
        Food::x = xp;
        Food::y = yp;
    }
    ~Food(){
        std::cout << "\nDestructor executed" << std::endl;
    }
};

class Snake: public Food{
public:
    int xPrev;
    int yPrev;
    Snake *next;
    // Very Very Important: constructor inheritence
    using Food::Food;
};

void grow(Snake **tail,int r, int g, int b){
    Snake *newtail = new Snake((*tail)->xPrev,(*tail)->yPrev);

    newtail->next = NULL;
    newtail->setFillColor(Color(r, g, b, 255));
    (*tail)->next = newtail;
    (*tail) = newtail;
}


void headMove(Snake *head_ptr,Direction *d){
            if(d->xIn){
                head_ptr->xPrev = head_ptr->x;
                head_ptr->yPrev = head_ptr->y;
                head_ptr->x+=10;
            }
            if(d->yIn){
                head_ptr->xPrev = head_ptr->x;
                head_ptr->yPrev = head_ptr->y;
                head_ptr->y+=10;
            }
            if(d->xDe){
                head_ptr->xPrev = head_ptr->x;
                head_ptr->yPrev = head_ptr->y;
                head_ptr->x-=10;
            }
            if(d->yDe){
                head_ptr->xPrev = head_ptr->x;
                head_ptr->yPrev = head_ptr->y;
                head_ptr->y-=10;
            }
}


void position_set(Snake *head_ptr){
    Snake *current = head_ptr;
    current->setPos();
    while(current->next != NULL){
        (current->next)->xPrev = (current->next)->x;
        (current->next)->x = current->xPrev;
        (current->next)->yPrev = (current->next)->y;
        (current->next)->y = current->yPrev;

        current = current->next;
        current->setPos();
    }
}

void draw_snake(Snake *head_ptr,RenderWindow *window){

        Snake *current = head_ptr;

        while(current!=NULL){
            window->draw(*(current));
            current = current->next;
        }
}

void deleteList( Snake* current_node){
    Snake* next_node;
    if(current_node->next != NULL){
        next_node = current_node->next;
        deleteList(next_node);
        delete current_node;
            std::cout << "Successful" << std::endl;

    }else{
        delete current_node;
            std::cout << "Successful" << std::endl;
    }
}

void xyControl(Snake *head_ptr){
    if( head_ptr->x > w-10){
        head_ptr->x = 0;
    }
    if( head_ptr->y > h-10){
        head_ptr->y = 0;
    }
    if( head_ptr->x < 0){
        head_ptr->x = w-10;
    }
    if( head_ptr->y < 0){
        head_ptr->y = h-10;
    }
}

int main()
{
    int n = 0;
    Direction d;

    RenderWindow window(VideoMode(w,h), "SFML works!");

    int r = 200,g = 40,b =0,sr=1,sg=-1,sb=1;


    Snake head(0,0);
    head.setFillColor(Color(r, g, 0, 255));
    head.next = NULL;

    Snake *tail = &head;

    Food food(w/2,h/2);
    food.setFillColor(Color::Green);

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        direction(&d);
        if(n%SPEED == 0){
            n=1;
            headMove(&head,&d);
            xyControl(&head);
            if((head.x == food.x) && (head.y == food.y)){

                r+=sr*10;g+=sg*30;

                if(r<20 || r>200){
                    sr *= -1;
                    b+=sb*40;
                }
                if(g<20 || g>200){
                    sg *= -1;
                    b+=sb*25;
                }
                if(b<20 || b>200){
                    sb *= -1;
                }
                grow(&tail,r,g,b);

                food.x = randomNumber(w/10)*10;
                food.y = randomNumber(h/10)*10;
            }
            position_set(&head);
            food.setPos();
        }
        window.clear();
        window.draw(food);
        draw_snake(&head,&window);
        window.display();
        n++;

        if(Keyboard::isKeyPressed(Keyboard::Q)){
            deleteList(&head);
            break;
        }
    }
    return 0;
}
