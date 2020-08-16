#include "Entity.h"

Entity::Entity()
{
    position = glm::vec3(0);
    movement = glm::vec3(0);
    acceleration = glm::vec3(0);
    velocity = glm::vec3(0);
    
    speed = 0;
    
    modelMatrix = glm::mat4(1.0f);
}

bool Entity::CheckCollision(Entity* other){
    if(isActive == false || other->isActive == false) return false;
    float xdist = fabs(position.x - other->position.x) - ((width + other->width) / 2.0f);
    float ydist = fabs(position.y - other->position.y) - ((height + other->height) / 2.0f);
    
    if (xdist < 0 && ydist < 0) return true;
    return false;
}

void Entity::CheckCollisionsY(Entity* objects, int objectCount){
    for(int i = 0; i < objectCount; i++){
        Entity* object = &objects[i];
        if(CheckCollision(object)){
            float ydist = fabs(position.y - object->position.y);
            float penetrationY = fabs(ydist - (height / 2.0f) - (object->height / 2.0f));
            if(velocity.y > 0){
                position.y -= penetrationY;
                velocity.y = 0;
                collidedTop = true;
                if(entityType == PLAYERBULLET){
                    isActive = false;
                    object->health -= dps;
                    if(object->health <= 0){
                        object->isActive = false;
                    }
                }
                if(entityType == PLAYER){
                    health -= object->dps;
                    if(health <= 0){
                        isActive = false;
                    }
                    object->isActive = false;
                }
            }else if(velocity.y < 0){
                position.y += penetrationY;
                velocity.y = 0;
                collidedBottom = true;
                if(entityType == PLAYERBULLET){
                    isActive = false;
                    object->health -= dps;
                    if(object->health <= 0){
                        object->isActive = false;
                    }
                }
                if(entityType == PLAYER){
                    health -= object->dps;
                    if(health <= 0){
                        isActive = false;
                    }
                    object->isActive = false;
                }
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
                if(entityType == PLAYERBULLET){
                    isActive = false;
                    object->health -= dps;
                    if(object->health <= 0){
                        object->isActive = false;
                    }
                }
                if(entityType == PLAYER){
                    health -= object->dps;
                    if(health <= 0){
                        isActive = false;
                    }
                    object->isActive = false;
                }
            }else if(velocity.x < 0){
                position.x += penetrationX;
                velocity.x = 0;
                collidedLeft = true;
                if(entityType == PLAYERBULLET){
                    isActive = false;
                    object->health -= dps;
                    if(object->health <= 0){
                        object->isActive = false;
                    }
                }
                if(entityType == PLAYER){
                    health -= object->dps;
                    if(health <= 0){
                        isActive = false;
                    }
                    object->isActive = false;
                }
            }
        }
    }
}

void Entity::CheckCollisionsY(Map *map)
{
 // Probes for tiles
    glm::vec3 top = glm::vec3(position.x, position.y + (height / 2), position.z);
    glm::vec3 top_left = glm::vec3(position.x - (width / 2), position.y + (height / 2), position.z);
    glm::vec3 top_right = glm::vec3(position.x + (width / 2), position.y + (height / 2), position.z);

    glm::vec3 bottom = glm::vec3(position.x, position.y - (height / 2), position.z);
    glm::vec3 bottom_left = glm::vec3(position.x - (width / 2), position.y - (height / 2), position.z);
    glm::vec3 bottom_right = glm::vec3(position.x + (width / 2), position.y - (height / 2), position.z);

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
    }
    else if (map->IsSolid(bottom_left, &penetration_x, &penetration_y) && velocity.y < 0) {
        position.y += penetration_y;
        velocity.y = 0;
        collidedBottom = true;
    }
    else if (map->IsSolid(bottom_right, &penetration_x, &penetration_y) && velocity.y < 0) {
        position.y += penetration_y;
        velocity.y = 0;
        collidedBottom = true;
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
         velocity.x = 0;
         collidedLeft = true;
     }

     if (map->IsSolid(right, &penetration_x, &penetration_y) && velocity.x > 0) {
         position.x -= penetration_x;
         velocity.x = 0;
         collidedRight = true;
     }
}

void Entity::CheckCollisionEdge(float horizontalX){
    float leftEdge = -horizontalX;
    if(entityType == TURRENTBULLET){
        if(position.x <= leftEdge - 5){
            isActive = false;
        }
        else if(position.x >= leftEdge + 5){
            isActive = false;
        }
        else if(position.y >= -0){
            isActive = false;
        }
        else if(position.y <= -7.5){
            isActive = false;
        }
    }
    else if(position.x <= leftEdge - 4.5){
        position.x = leftEdge - 4.5;
        if(entityType == PLAYERBULLET){
            isActive = false;
        }
    }
    else if(position.x >= leftEdge + 4.5){
        position.x = leftEdge + 4.5;
        if(entityType == PLAYERBULLET){
            isActive = false;
        }
    }
    else if(position.y >= -0.8){
        position.y = -0.8;
        if(entityType == PLAYERBULLET){
            isActive = false;
        }
    }
    else if(position.y <= -6.7){
        position.y = -6.7;
        if(entityType == PLAYERBULLET){
            isActive = false;
        }
    }
}

void Entity::AIWalker(){
    movement = glm::vec3(-1, 0, 0);
}

void Entity::AIWaitAndGO(Entity* player){
    switch(aiState){
        case IDLE:
            if(glm::distance(position, player->position) < 7.0f){
                aiState = WALKING;
            }
            break;
        case WALKING:
            if(player->position.x < position.x){
                movement = glm::vec3(-1, 0, 0);
            }else{
                movement = glm::vec3(1, 0, 0);
            }
            if(player->position.y < position.y){
                movement = glm::vec3(0, -1, 0);
            }else{
                movement = glm::vec3(0, 1, 0);
            }
            break;
        case ATTACKING:
            break;
    }
}

void Entity::AI(Entity* player){
    switch(aiType){
        case WALKER:
            AIWalker();
            break;
        case WAITNGO:
            AIWaitAndGO(player);
            break;
    }
}

void Entity::turrentUpdate(Entity* player, Entity* turrentBullets){
    float movementX = 0;
    float movementY = 0;
    if(position.x - player->position.x < 1){
        animIndices = animRight;
        shoot = true;
    }
    else if(position.x - player->position.x > 6){
        animIndices = animLeft;
        shoot = false;
        shoot = 0;
    }else if(position.x - player->position.x < 6 ){
        animIndices = animStill;
        shoot = true;
    }
    if(position.x < player->position.x){
        shoot = false;
    }
    
    if(shoot == true){
        shootInterval += 0.005;
        if(shootInterval >= 1){
            if(location == 1){
                movementX = -(position.x - player->position.x);
                movementY = -(position.y - player->position.y);
            }else if(location == -1){
                movementX = -(position.x - player->position.x);
                movementY = -(position.y - player->position.y);
            }
            for(int i = 0; i < 50; i++){
                if(turrentBullets[i].isActive == false){
                    turrentBullets[i].isActive = true;
                    turrentBullets[i].position = glm::vec3(position.x, position.y, 0);
                    turrentBullets[i].movement = glm::vec3(movementX, movementY, 0);
                    break;
                }
            }
            shootInterval = 0;
        }
    }
}

void Entity::Update(float deltaTime, Entity* player, Entity* objects, int objectCount, Map* map, float horizontalX, Entity* turrentBullets)
{
    if(isActive == false) return;
    
    collidedTop = false;
    collidedBottom = false;
    collidedLeft = false;
    collidedRight = false;
    
    if(entityType == ENEMY){
        AI(player);
    }
    
    if (animIndices != NULL) {
        if (glm::length(movement) != 0) {
            animTime += deltaTime;

            if (animTime >= 0.25f)
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
    
    if(entityType == PLAYER || entityType == PLAYERBULLET || entityType == TURRENTBULLET){
        CheckCollisionEdge(horizontalX);
    }
    
    if(entityType == PLAYER){
        CheckCollisionsX(objects, objectCount);
        CheckCollisionsY(objects, objectCount);
    }
    
    if(entityType == PLAYERBULLET){
        CheckCollisionsX(objects, objectCount);
    }
    
    if(entityType == PLAYER && health <= 0){
        isActive = false;
        std::cout << "you died" << std::endl;
    }
    
    if(entityType == TURRENTBULLET){
        if(CheckCollision(player)){
            isActive = false;
            player->health -= dps;
        }
    }
    
    if(entityType == TURRENT){
        turrentUpdate(player, turrentBullets);
    }
    
    velocity = movement * speed;
    velocity += acceleration * deltaTime;
    
    position.y += velocity.y * deltaTime; // Move on Y
//    CheckCollisionsY(map);
//    CheckCollisionsY(objects, objectCount); // Fix if needed

    position.x += velocity.x * deltaTime; // Move on X
//    CheckCollisionsX(map);
//    CheckCollisionsX(objects, objectCount); // Fix if needed
    
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
