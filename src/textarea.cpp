/*
    Copyright (C) 2008 Andrew Caudwell (acaudwell@gmail.com)

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version
    3 of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "textarea.h"

TextArea::TextArea() {
}

TextArea::TextArea(FXFont font) {
    this->colour = vec3(1.0f,1.0f,1.0f);
    this->font   = font;
    this->corner = vec2(0.0f,0.0f);
    this->visible = false;

    this->font.dropShadow(true);
}

TextArea::TextArea(std::vector<std::string>& content, FXFont font, vec3 colour) {
    this->colour = colour;
    this->font   = font;
    this->corner = vec2(0.0f,0.0f);
    setText(content);
}

void TextArea::hide() {
    this->visible=false;
}

int TextArea::getMaxCharacters() {
    return (display.width / font.getMaxWidth()) * 0.8;
}

void TextArea::setColour(vec3 colour) {
    this->colour = colour;
}

void TextArea::setText(const std::vector<std::string>& content_) {
    this->content.clear();

    //calculate area
    rectwidth  = 0;
    rectheight = content_.size() * (font.getMaxHeight()+4) + 2;

    size_t max_characters = getMaxCharacters();

    for(std::string s : content_) {

        if(s.size() > max_characters) {
            s = s.substr(0,max_characters);
        }

        int width = font.getWidth(s) + 6;
        if(width>rectwidth) rectwidth = width;
        this->content.push_back(s);
    }
    this->visible=true;
}

void TextArea::setPos(vec2 pos) {

    corner = pos;

    int fontheight = font.getMaxHeight() + 4;

    corner.y -= rectheight;

    if((corner.x + rectwidth) > display.width) {
        if((corner.x - rectwidth - fontheight )>0) {
            corner.x -= rectwidth;
        } else {
            corner.x = display.width - rectwidth;
        }
    }

    if(corner.y < 0) corner.y += rectheight + fontheight;
    if(corner.y +rectheight > display.height) {
        corner.y = std::max(0.0f, corner.y-rectheight);
    }

}

void TextArea::draw() {
    if(!visible) return;

    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    vec4 col(colour*0.2f, 0.6f);

    glColor4fv(glm::value_ptr(col));
    glBegin(GL_QUADS);
        glVertex2f(corner.x,           corner.y);
        glVertex2f(corner.x,           corner.y + rectheight);
        glVertex2f(corner.x+rectwidth, corner.y + rectheight);
        glVertex2f(corner.x+rectwidth, corner.y);
    glEnd();

    glEnable(GL_TEXTURE_2D);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    int yinc = 2;

    std::vector<std::string>::iterator it;
    for(it = content.begin(); it != content.end(); it++) {
        font.draw((int)corner.x+2, (int)corner.y+yinc,  (*it).c_str());
        yinc += font.getMaxHeight() + 4;
    }
}
