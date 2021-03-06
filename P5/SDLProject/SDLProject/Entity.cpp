#include "Entity.h"

Entity::Entity()
{
    position = glm::vec3(0);
    movement = glm::vec3(0);
    acceleration = glm::vec3(0);
    velocity = glm::vec3(0);
    speed = 0;
    modelMatrix = glm::mat4(1.0f);
    bounce = Mix_LoadWAV("bounce.wav");
    land = Mix_LoadWAV("landing.wav");
}

bool Entity::CheckCollision(Entity* other){
    if(isActive == false || other->isActive == false) return false;
    float xdist = fabs(position.x - other->position.x) - ((width + other->width) / 2.0f);
    float ydist = fabs(position.y - other->position.y) - ((height + 0.8 + other->height) / 2.0f);
    
    if (xdist < 0 && ydist < 0) return true;
    return false;
}

void Entity::CheckCollisionsY(Entity* objects, int objectCount){
    for(int i = 0; i < objectCount; i++){
        Entity* object = &objects[i];
        if(CheckCollision(object)){
            float ydist = fabs(position.y - object->position.y - 0.5);
            float penetrationY = fabs(ydist - (height / 2.0f) - (object->height / 2.0f));
            if(velocity.y > 0){
                position.y -= penetrationY;
                velocity.y = 0;
                collidedTop = true;
            }else if(velocity.y < 0){
                position.y += penetrationY;
                velocity.y = 0;
                collidedBottom = true;
            }
        }
    }
}

void Entity::CheckCollisionsX(Entity* objects, int objectCount){
    for(int i = 0; i < objectCount; i++){
        Entity* object = &objects[i];
        if(CheckCollision(object)){
            float xdist = fabs(position.x - object->position.x);
            float penetrationX = fabs(xdist - (width / 2.0f) - (object->width / 2.0f));
            if(velocity.x > 0){
                position.x -= penetrationX;
                velocity.x = 0;
                collidedRight = true;
                collidedRightE = true;
            }else if(velocity.x < 0){
                position.x += penetrationX;
                velocity.x = 0;
                collidedLeft = true;
                collidedLeftE = true;
            }
        }
    }
}

void Entity::CheckCollisionsY(Map *map)
{
 // Probes for tiles
    glm::vec3 top = glm::vec3(position.x, position.y + (height / 2) - 0.2f, position.z);
    glm::vec3 top_left = glm::vec3(position.x - 0.1f, position.y + (height / 2) - 0.2f, position.z);
    glm::vec3 top_right = glm::vec3(position.x + 0.1f, position.y + (height / 2) - 0.2f, position.z);

    glm::vec3 bottom = glm::vec3(position.x, position.y - (height / 2), position.z);
    glm::vec3 bottom_left = glm::vec3(position.x, position.y - (height / 2), position.z);
    glm::vec3 bottom_right = glm::vec3(position.x, position.y - (height / 2), position.z);

    float penetration_x = 0;
    float penetration_y = 0;
    
    if (map->IsSolid(top, &penetration_x, &penetration_y) && velocity.y > 0) {
        position.y -= penetration_y;
        velocity.y = 0;
        collidedTop = true;
    }
    else if (map->IsSolid(top_left, &penetration_x, &penetration_y) && velocity.y > 0) {
        position.y -= penetration_y;
        velocity.y = 0;
        collidedTop = true;
    }
    else if (map->IsSolid(top_right, &penetration_x, &penetration_y) && velocity.y > 0) {
        position.y -= penetration_y;
        velocity.y = 0;
        collidedTop = true;
    }
    if (map->IsSolid(bottom, &penetration_x, &penetration_y) && velocity.y < 0) {
        position.y += penetration_y;
        velocity.y = 0;
        collidedBottom = true;
        if(inAir == true){
            Mix_PlayChannel(-1, land, 0);
        }
        velocity.x = 0;
        if(!chargeJump){
            if(lastOrientation == 1){
                animIndices = animRight;
            }else{
                animIndices = animLeft;
            }
        }
        inAir = false;
    }
    else if (map->IsSolid(bottom_left, &penetration_x, &penetration_y) && velocity.y < 0) {
        position.y += penetration_y;
        velocity.y = 0;
        collidedBottom = true;
        if(inAir == true){
            Mix_PlayChannel(-1, land, 0);
        }
        velocity.x = 0;
        if(!chargeJump){
            if(lastOrientation == 1){
                animIndices = animRight;
            }else{
                animIndices = animLeft;
            }
        }
        inAir = false;
    }
    else if (map->IsSolid(bottom_right, &penetration_x, &penetration_y) && velocity.y < 0) {
        position.y += penetration_y;
        velocity.y = 0;
        collidedBottom = true;
        if(inAir == true){
            Mix_PlayChannel(-1, land, 0);
        }
        velocity.x = 0;
        if(!chargeJump){
            if(lastOrientation == 1){
                animIndices = animRight;
            }else{
                animIndices = animLeft;
            }
        }
        inAir = false;
    }
}

void Entity::CheckCollisionsX(Map *map)
{
 // Probes for tiles
     glm::vec3 left = glm::vec3(position.x - (width / 2), position.y, position.z);
     glm::vec3 right = glm::vec3(position.x + (width / 2), position.y, position.z);

     float penetration_x = 0;
     float penetration_y = 0;
     if (map->IsSolid(left, &penetration_x, &penetration_y) && velocity.x < 0) {
         position.x += penetration_x;
         if(entityType == PLAYER){
             Mix_PlayChannel(-1, bounce, 0);
             velocity.x = - velocity.x / 2;
         }
         collidedLeft = true;
     }

     if (map->IsSolid(right, &penetration_x, &penetration_y) && velocity.x > 0) {
         position.x -= penetration_x;
         if(entityType == PLAYER){
             Mix_PlayChannel(-1, bounce, 0);
             velocity.x = - velocity.x / 2;
         }
         collidedRight = true;
     }
}

void Entity::CheckCollisionsXE(Map *map)
{
 // Probes for tiles
     glm::vec3 left = glm::vec3(position.x - (width / 2) - 0.1, position.y, position.z);
     glm::vec3 right = glm::vec3(position.x + (width / 2) + 0.1, position.y, position.z);

     float penetration_x = 0;
     float penetration_y = 0;
     if (map->IsSolid(left, &penetration_x, &penetration_y)) {
         position.x += penetration_x;
         collidedLeft = true;
     }

     if (map->IsSolid(right, &penetration_x, &penetration_y)) {
         position.x -= penetration_x;
         collidedRight = true;
     }
}

void Entity::CheckCollisionsYE(Map *map)
{
    glm::vec3 bottom = glm::vec3(position.x, position.y - (height / 2) - 0.2, position.z);
    glm::vec3 bottom_left = glm::vec3(position.x + 1, position.y - (height / 2) - 0.2, position.z);
    glm::vec3 bottom_right = glm::vec3(position.x + 4.5, position.y - (height / 2) - 0.2, position.z);

    float penetration_x = 0;
    float penetration_y = 0;
    if (map->IsSolid(bottom, &penetration_x, &penetration_y)) {
        collidedBottom = true;
    }
    else if (map->IsSolid(bottom_left, &penetration_x, &penetration_y)) {
        collidedBottom = true;
    }
    else if (map->IsSolid(bottom_right, &penetration_x, &penetration_y)) {
        collidedBottom = true;
    }
}

bool Entity::CheckCollision(Map* map){
    CheckCollisionsX(map);
    CheckCollisionsYE(map);
    if(collidedLeft || collidedRight || collidedTop || collidedBottom){
        return true;
    }else{
        return false;
    }
}

void Entity::AIWalker(Map* map){
    CheckCollisionsXE(map);
    CheckCollisionsYE(map);
    switch (aiState){
        case IDLE:
            break;
        case WALKING:
            if(lastOrientation == -1){
                if(!collidedBottom){
                    lastOrientation = 1;
                }
                if(collidedLeft){
                    lastOrientation = 1;
                }else{
                    movement = glm::vec3(-0.05, 0, 0);
                }
                
            }else{
                if(!collidedBottom){
                    lastOrientation = -1;
                }
                else if(collidedRight){
                    lastOrientation = -1;
                }else{
                    movement = glm::vec3(0.05,0,0);
                }
            }
            break;
        case ATTACKING:
            break;
    }
}

void Entity::AIWaitAndGO(Entity* player, Map* map){
    switch(aiState){
        case IDLE:
            break;
        case WALKING:
            break;
        case ATTACKING:
            break;
    }
}

void Entity::AI(Entity* player, Map* map){
    switch(aiType){
        case WALKER:
            AIWalker(map);
            break;
        case WAITNGO:
            AIWaitAndGO(player, map);
            break;
    }
}

void Entity::Update(float deltaTime, Entity* player, Entity* objects, int objectCount, Map* map)
{
    if(isActive == false) return;
    
    collidedTop = false;
    collidedBottom = false;
    collidedLeft = false;
    collidedRight = false;
    collidedRightE = false;
    collidedLeftE = false;
    
    if(entityType == ENEMY){
        AI(player, map);
    }
    
    if (animIndices != NULL) {
        if (glm::length(movement) != 0 || alwaysAnim == true) {
            animTime += deltaTime;

            if (animTime >= animSpeed)
            {
                animTime = 0.0f;
                animIndex++;
                if (animIndex >= animFrames)
                {
                    animIndex = 0;
                }
            }
        } else {
            animIndex = 0;
        }
    }
    if(entityType == ENEMY){
        velocity += acceleration * deltaTime;
        velocity.x = movement.x * speed * 3;
        position.x += velocity.x * deltaTime;
        CheckCollisionsX(map);
        
//        position.y += velocity.y * deltaTime;
//        CheckCollisionsY(map);
        
    }
    if(entityType == PLAYER){
        velocity += acceleration * deltaTime * canMove;
        
        position.y += velocity.y * deltaTime * canMove; // Move on Y
        CheckCollisionsY(map);
        CheckCollisionsY(objects, objectCount); // Fix if needed

        position.x += velocity.x * deltaTime * canMove; // Move on X
        CheckCollisionsX(map);
        CheckCollisionsX(objects, objectCount); // Fix if needed
    }
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
}

void Entity::DrawSpriteFromTextureAtlas(ShaderProgram *program, GLuint textureID, int index)
{
    float u = (float)(index % animCols) / (float)animCols;
    float v = (float)(index / animCols) / (float)animRows;
    
    float width = 1.0f / (float)animCols;
    float height = 1.0f / (float)animRows;
    
    float texCoords[] = { u, v + height, u + width, v + height, u + width, v,
        u, v + height, u + width, v, u, v};
    
    float vertices[]  = { -0.5f* renderSize, -0.5f* renderSize, 0.5f* renderSize, -0.5f* renderSize, 0.5f* renderSize, 0.5f* renderSize, -0.5f* renderSize, -0.5f* renderSize, 0.5f* renderSize, 0.5f* renderSize, -0.5f* renderSize, 0.5f* renderSize };
    
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

void Entity::Render(ShaderProgram *program) {
    if(isActive == false) return;

    program->SetModelMatrix(modelMatrix);
    
    if (animIndices != NULL) {
        DrawSpriteFromTextureAtlas(program, textureID, animIndices[animIndex]);
        return;
    }
    
    float vertices[]  = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
    
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

bool Entity::isAlive(){
    if(lives < 1){
        return false;
    }
    return true;
}

void Entity::CheckDamage(Entity* enemies, int enemyCount){
    CheckCollisionsX(enemies, enemyCount);
    CheckCollisionsY(enemies, enemyCount);
    if(collidedLeftE || collidedRightE){
        position = startPt;
        lives -= 1;
    }
//    for(int i = 0; i < enemyCount; i++){
//        if(enemies[i].isActive == true && CheckCollision(&enemies[i])){
//            position = startPt;
//            lives -= 1;
//        }
//        if(lives <= 0){
//            isActive = false;
//        }
//    }
}
