#ifndef BASE_OBJECT_H
#define BASE_OBJECT_H

#include "func.h"

class baseobj
{
public:
    baseobj();
    ~baseobj();
    void SetRect( const int& x, const int& y)
    {
        rect_.x = x;
        rect_.y = y;
    }
    SDL_Rect GetRect() const { return rect_;}

    virtual bool LoadImg(std::string path, SDL_Renderer* renderer);

    void Render(SDL_Renderer* des, const SDL_Rect* clip = NULL);
    void Free();
protected:
    SDL_Texture* p_object_;
    SDL_Rect rect_;
    SDL_Texture* texture;
};
#endif // BASE_OBJECT_H_
