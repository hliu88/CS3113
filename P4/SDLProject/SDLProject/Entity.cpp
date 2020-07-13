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

bool Entity::CheckCollisionSword(Entity* other){
    if(isActive == false || other->isActive == false) return false;
    if(lastOrientation == 0 && position.x < other->position.x){
        return false;
    }else if(lastOrientation == 1 && position.x > other->position.x){
        return false;
    }
//        else if(position.y - other->position.y > 0.5f || other->position.y - position.y < 0.5f){
//        return false;
//    }
    float xdist = fabs(position.x - other->position.x) - ((width + other->width) / 2.0f);
    float ydist = fabs(position.y - other->position.y) - ((height + other->height) / 2.0f);
    
    if (xdist < 0 && ydist < 0) return true;
    return false;
}

void Entity::CheckHitReg(Entity* enemies, int enemyCount){
    for(size_t i = 0; i < enemyCount; i++){
        Entity* enemy = &enemies[i];
        if(CheckCollisionSword(enemy)){
            enemy->isActive = false;
        }
    }
    
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
            }else if(velocity.x < 0){
                position.x += penetrationX;
                velocity.x = 0;
                collidedLeft = true;
            }
        }
    }
}

void Entity::AIWalker(){
    lastOrientation = -1;
    switch(aiState){
        case WALKING:
                movement = glm::vec3(-1 * speed, 0, 0);
            break;
        case ATTACKING:
            break;
    }
}

void Entity::AIWaitAndGO(Entity* player){
    switch(aiState){
        case IDLE:
            if(glm::distance(position, player->position) < 3.0f){
                aiState = WALKING;
            }
            break;
        case WALKING:
            if(player->position.x < position.x){
                animIndices = animLeft;
                movement = glm::vec3(-1 * speed, 0, 0);
            }else{
                animIndices = animRight;
                movement = glm::vec3(1 * speed, 0, 0);
            }
            break;
        case ATTACKING:
            break;
    }
}

void Entity::CheckPenetration(Entity* objects, int objectCount){
    for(int i = 0; i < objectCount; i++){
        Entity* object = &objects[i];
        if(CheckCollision(object)){
            penetration = true;
        }
    }
}

void Entity::AIPatrol(Entity* player, Entity* platforms, int platformCount){
    Entity sensorRight, sensorLeft;
    sensorRight.position = glm::vec3(position.x + 0.5f, position.y - 0.6f, 0);
    sensorLeft.position = glm::vec3(position.x - 0.5f, position.y - 0.6f, 0);
    if(lastOrientation == 1){
        sensorRight.CheckPenetration(platforms, platformCount);
        if(!sensorRight.penetration){
            lastOrientation = -1;
            animIndices = animLeft;
        }
    }
    if(lastOrientation == -1){
        sensorLeft.CheckPenetration(platforms, platformCount);
        if(!sensorLeft.penetration){
            lastOrientation = 1;
            animIndices = animRight;
        }
    }
    switch (aiState){
        case WALKING:
            movement = glm::vec3(lastOrientation * speed, 0, 0);
            break;
        case ATTACKING:
            break;
    }
}


void Entity::AI(Entity* player, Entity* platforms, int platformCount){
    switch(aiType){
        case WALKER:
            AIWalker();
            break;
        case WAITNGO:
            AIWaitAndGO(player);
            break;
        case PATROL:
            AIPatrol(player, platforms, platformCount);
            break;
    }
}

void Entity::CheckDamage(Entity* enemies, int enemyCount){
    for(int i = 0; i < enemyCount; i++){
        if(enemies[i].isActive == true && CheckCollision(&enemies[i])){
            position = glm::vec3(-4.5,-1.5,0);
            enemies[i].movement.x += 0.8 * -lastOrientation;
            life -= 1;
        }
        if(life <= 0){
            isActive = false;
        }
    }
}

void Entity::Update(float deltaTime, Entity* player, Entity* platforms, int platformCount, Entity* enemies, int enemyCount)
{
    if(isActive == false) return;
    
    collidedTop = false;
    collidedBottom = false;
    collidedLeft = false;
    collidedRight = false;
    
    if(entityType == ENEMY){
        AI(player, platforms, platformCount);
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
    
    if(jump){
        jump = false;
        inAir = true;
        velocity.y += jumpPower;
    }
    
    if(entityType == PLAYER){
        CheckDamage(enemies, enemyCount);
    }
    
    velocity.x = movement.x * speed;
    velocity += acceleration * deltaTime;
    
    position.y += velocity.y * deltaTime;
    CheckCollisionsY(platforms, platformCount);
    
    
    position.x += velocity.x * deltaTime;
    CheckCollisionsX(platforms, platformCount);
    
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
