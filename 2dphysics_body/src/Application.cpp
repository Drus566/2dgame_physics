#include "./Application.h"
#include "./Physics/Constans.h"
#include "./Physics/Force.h"
#include "./Physics/CollisionDetection.h"
#include "./Physics/Contact.h"

bool Application::IsRunning() {
    return running;
}

void Application::Setup() {
    running = Graphics::OpenWindow();

    Body* floor = new Body(BoxShape(Graphics::Width() - 50, 50), Graphics::Width() / 2.0, Graphics::Height() - 50, 0.0);
    floor->restitution = 0.2;
    bodies.push_back(floor);

    Body* bigBox = new Body(BoxShape(200,200), Graphics::Width() / 2.0, Graphics::Height() / 2.0, 0.0);
    bigBox->rotation = 1.4;
    bigBox->restitution = 0.5;
    bodies.push_back(bigBox);

    // Body* bigBall = new Body(CircleShape(100),100,100,1.0);
    // Body* smallBall = new Body(CircleShape(50),500,100,1.0);
    // bodies.push_back(bigBall);
    // bodies.push_back(smallBall);
} 

void Application::Input() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) running = false;
                break;
            case SDL_MOUSEBUTTONDOWN:
                int x, y;
                SDL_GetMouseState(&x, &y);
                Body* box = new Body(BoxShape(50, 50), x, y, 1.0);
                bodies.push_back(box);
                // bodies[0]->position.x = x;
                // bodies[0]->position.y = y;
                break;

            // case SDL_MOUSEBUTTONDOWN:
            //     int x, y;
            //     SDL_GetMouseState(&x, &y);
            //     Body* smallBall = new Body(CircleShape(40), x, y, 1.0);
            //     smallBall->restitution = 0.9;
            //     bodies.push_back(smallBall);
            //     break;

            // case SDL_MOUSEMOTION:
            //     int x, y;
            //     SDL_GetMouseState(&x, &y);
            //     bodies[0]->position.x = x;
            //     bodies[0]->position.y = y;

            // case SDL_KEYDOWN:
            //     if (event.key.keysym.sym == SDLK_ESCAPE) running = false;
            //     if (event.key.keysym.sym == SDLK_UP) pushForce.y = -50 * PIXELS_PER_METER;
            //     if (event.key.keysym.sym == SDLK_RIGHT) pushForce.x = 50 * PIXELS_PER_METER;
            //     if (event.key.keysym.sym == SDLK_DOWN) pushForce.y = 50 * PIXELS_PER_METER;
            //     if (event.key.keysym.sym == SDLK_LEFT) pushForce.x = -50 * PIXELS_PER_METER;
            //     break;
            // case SDL_KEYUP:
            //     if (event.key.keysym.sym == SDLK_UP) pushForce.y = 0;
            //     if (event.key.keysym.sym == SDLK_RIGHT) pushForce.x = 0;
            //     if (event.key.keysym.sym == SDLK_DOWN) pushForce.y = 0;
            //     if (event.key.keysym.sym == SDLK_LEFT) pushForce.x = 0;
            //     break;
            // // case SDL_MOUSEBUTTONDOWN:
            // //     if (event.button.button == SDL_BUTTON_LEFT) {
            // //         int x, y;
            // //         SDL_GetMouseState(&x, &y);
            // //         body* body = new body(x, y, 1.0);
            // //         body->radius = 5;
            // //         bodys.push_back(body);
            // //     }
            // //     break;
            // case SDL_MOUSEMOTION:
            //     mouseCursor.x = event.motion.x;
            //     mouseCursor.y = event.motion.y;
            // case SDL_MOUSEBUTTONDOWN:
            //     if (!leftMouseButtonDown && event.button.button == SDL_BUTTON_LEFT) {
            //         leftMouseButtonDown = true;
            //         int x, y;
            //         SDL_GetMouseState(&x, &y);
            //         mouseCursor.x = x;
            //         mouseCursor.y = y;
            //     }
            //     break;
            // case SDL_MOUSEBUTTONUP:
            //     if (leftMouseButtonDown && event.button.button == SDL_BUTTON_LEFT) {
            //         leftMouseButtonDown = false;
            //         int lastbody = NUM_BODIES - 1;
            //         Vec2 impulseDirection = (bodies[lastbody]->position - mouseCursor).UnitVector();
            //         float impulseMagnitude = (bodies[lastbody]->position - mouseCursor).Magnitude() * 5.0;
            //         bodies[lastbody]->velocity = impulseDirection * impulseMagnitude;

            //         // Vec2 impulseDirection = (bodys[0]->position - mouseCursor).UnitVector();
            //         // float impulseMagnitude = (bodys[0]->position - mouseCursor).Magnitude() * 5.0;
            //         // bodys[0]->velocity = impulseDirection * impulseMagnitude;
            //     }
            //     break;
        }
    }
}

void Application::Update() {
    Graphics::ClearScreen(0xFF0F0721);


    // Wait some time until reach the target frame time in milliseconds, 
    // (SDL_GetTicks() - timePreviousFrame) это время за которое отрисовался кадр,
    // если оно больше, то сразу отрисовываем кадр,
    // если заложенное нами время на отрисовку кадра больше (MILLISECS_PER_FRAME),
    // то ждем оставшуюся разницу по времени, чтобы отрисовать кадр во время  
    static int timePreviousFrame;
    int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - timePreviousFrame);
    if (timeToWait > 0) {
        SDL_Delay(timeToWait);
    }
    
    // body->velocity = Vec2(2.0, 0.0);
    // body->position += body->velocity;

    // Вместо того чтобы считать на сколько пикселей передвигается в один кадр (FPS),
    // мы считаем как много пикселей передвигается в секунду, благодаря deltaTime
    // Дельта тайм это время прошедшее с момента предыдущего кадра до текущего кадра (в секундах)
    // Помогает достичь на разных пк одинакового поведения отрисовки пикселей (например перемещение)
    // Потому что FPS может менятся от производительности железа ПК.
    float deltaTime = (SDL_GetTicks() - timePreviousFrame) / 1000.0f;
    // Если вдруг дебажим код, то приводим дельта тайм к корректному значению
    // 0.016 это взято с расчетом что,
    // const int FPS = 60;
    // const int MILLISECS_PER_FRAME = 1000 / FPS; // сколько миллисекунд должно занимать завершение каждого кадра в секунде)
    // и это число делить на 1000.0f получается 0.016
    if (deltaTime > 0.016) {
        deltaTime = 0.016;
    }

    timePreviousFrame = SDL_GetTicks();

    // bodies[0]->AddForce(pushForce);

    // body->acceleration = Vec2(0.0, 9.8 * PIXELS_PER_METER);
    for (auto body : bodies) {
    //     // Vec2 wind = Vec2(0.2 * PIXELS_PER_METER, 0.0); // ветер
    //     // body->AddForce(wind);
        Vec2 weight = Vec2(0.0, body->mass * 9.8 * PIXELS_PER_METER); // Сила тяжести
        body->AddForce(weight);
    //     // body->AddForce(pushForce); // Сила из клавиатуры

    //     // Vec2 drag = Force::GenerateDragForce(*body, 0.003);
    //     // body->AddForce(drag);

    //     Vec2 weight = Vec2(0.0, body->invMass * 9.8 * PIXELS_PER_METER);
    //     body->AddForce(weight);

    //     Vec2 wind = Vec2(2.0 * PIXELS_PER_METER, 0.0);
    //     body->AddForce(wind);

    //     // // Только если в воде
    //     // // if (body->position.y >= liquid.y) {
    //     // //     Vec2 drag = Force::GenerateDragForce(*body, 0.01);
    //     // //     body->AddForce(drag);
    //     // // }

    //     // float torque = 200;
    //     // body->AddTorque(torque);
    }

    for (auto body : bodies) {
        body->Update(deltaTime);
    }

    // Check all rigidbodies with the other rigidbodies for collision
    for (int i = 0; i <= bodies.size() - 1; i++) {
        for (int j = i + 1; j < bodies.size(); j++) {
            // TODO: Check bodies[i] with bodies[j]
            Body* a = bodies[i];
            Body* b = bodies[j];
            a->isColliding = false;
            b->isColliding = false;
            Contact contact;
            if (CollisionDetection::isColliding(a, b, contact)) {
                contact.ResolveCollision();

                Graphics::DrawFillCircle(contact.start.x, contact.start.y, 3, 0xFFFF00FF);
                Graphics::DrawFillCircle(contact.end.x, contact.end.y, 3, 0xFFFF00FF);
                Graphics::DrawLine(contact.start.x, contact.start.y, contact.start.x + contact.normal.x * 15, contact.start.y + contact.normal.y * 15, 0xFFFF00FF);
                a->isColliding = true;
                b->isColliding = true;
            }
        }
    }

    // Проверка границ окна
    for (auto body : bodies) {
        if (body->shape->GetType() == CIRCLE) {
            CircleShape* circleShape = (CircleShape*) body->shape;
            // Если о левый борт
            if (body->position.x - circleShape->radius <= 0) {
                body->position.x = circleShape->radius;
                body->velocity.x *= -0.9; // меняем направление скорости с помощью знака -
            }
            // Если о правый борт 
            else if (body->position.x + circleShape->radius >= Graphics::Width()) {
                body->position.x = Graphics::Width() - circleShape->radius;
                body->velocity.x *= -0.9; 
            }
            // Если о потолок
            if (body->position.y - circleShape->radius <= 0) {
                body->position.y = circleShape->radius;
                body->velocity.y *= -0.9;
            }
            // Если о землю
            else if (body->position.y + circleShape->radius >= Graphics::Height()) {
                body->position.y = Graphics::Height() - circleShape->radius;
                body->velocity.y *= -0.9; 
            }
        }
    }
}

void Application::Render() {
    // Graphics::ClearScreen(0xFF0F0721);

    for (auto body: bodies) {
        Uint32 color = body->isColliding ? 0xFF0000FF : 0xFFFFFFFF;

        if (body->shape->GetType() == CIRCLE) {
            CircleShape* circleShape = (CircleShape*) body->shape;
            Graphics::DrawFillCircle(body->position.x, body->position.y, circleShape->radius, color);
        }
        if (body->shape->GetType() == BOX) {
            BoxShape* boxShape = (BoxShape*) body->shape;
            Graphics::DrawPolygon(body->position.x, body->position.y, boxShape->worldVertices, color); 
        }
    }

    // if (leftMouseButtonDown) {
    //     int lastbody = NUM_BODIES - 1;
    //     Graphics::DrawLine(bodies[lastbody]->position.x, bodies[lastbody]->position.y, mouseCursor.x, mouseCursor.y, 0xFF0000FF);
    // }

    // Graphics::DrawLine(bodies[0]->position.x, bodies[0]->position.y, bodies[1]->position.x, bodies[1]->position.y, 0xFF313131);
    // Graphics::DrawLine(bodies[1]->position.x, bodies[1]->position.y, bodies[2]->position.x, bodies[2]->position.y, 0xFF313131);
    // Graphics::DrawLine(bodies[2]->position.x, bodies[2]->position.y, bodies[3]->position.x, bodies[3]->position.y, 0xFF313131);
    // Graphics::DrawLine(bodies[3]->position.x, bodies[3]->position.y, bodies[0]->position.x, bodies[0]->position.y, 0xFF313131);
    // Graphics::DrawLine(bodies[0]->position.x, bodies[0]->position.y, bodies[2]->position.x, bodies[2]->position.y, 0xFF313131);
    // Graphics::DrawLine(bodies[1]->position.x, bodies[1]->position.y, bodies[3]->position.x, bodies[3]->position.y, 0xFF313131);

    // Graphics::DrawFillCircle(bodies[0]->position.x, bodies[0]->position.y, bodies[0]->radius, 0xFFEEBB00);
    // Graphics::DrawFillCircle(bodies[1]->position.x, bodies[1]->position.y, bodies[1]->radius, 0xFFEEBB00);
    // Graphics::DrawFillCircle(bodies[2]->position.x, bodies[2]->position.y, bodies[2]->radius, 0xFFEEBB00);
    // Graphics::DrawFillCircle(bodies[3]->position.x, bodies[3]->position.y, bodies[3]->radius, 0xFFEEBB00);


    // Draw the anchor and the spring to the first bob
    // Graphics::DrawFillCircle(anchor.x, anchor.y, 5, 0xFF001155);
    // Graphics::DrawLine(anchor.x, anchor.y, bodys[0]->position.x, bodys[0]->position.y, 0xFF313131);

    // // Draw all the springs from one body to the next
    // for (int i = 0; i < NUM_bodyS - 1; i++) {
    //     int currbody = i;
    //     int nextbody = i + 1;
    //     Graphics::DrawLine(bodys[currbody]->position.x, bodys[currbody]->position.y, bodys[nextbody]->position.x, bodys[nextbody]->position.y, 0xFF313131);
    // }

    // // Draw all the bob bodys
    // for (auto body: bodys) {
    //     Graphics::DrawFillCircle(body->position.x, body->position.y, body->radius, 0xFFEEBB00);
    // }

    Graphics::RenderFrame();
}

void Application::Destroy() {
    for (auto body : bodies) {
        delete body;
    }
    Graphics::CloseWindow();
}