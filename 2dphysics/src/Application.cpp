#include "./Application.h"
#include "./Physics/Constans.h"
#include "./Physics/Force.h"

bool Application::IsRunning() {
    return running;
}

void Application::Setup() {
    running = Graphics::OpenWindow();

    // anchor = Vec2(Graphics::Width() / 2.0, 30);

    // for (int i = 0; i < NUM_PARTICLES; i++) {
    //     Particle* bob = new Particle(anchor.x, anchor.y + (i * restLength), 2.0);
    //     bob->radius = 6;
    //     particles.push_back(bob);
    // }

    Particle* a = new Particle(100, 100, 1.0);
    Particle* b = new Particle(300, 100, 1.0);
    Particle* c = new Particle(300, 300, 1.0);
    Particle* d = new Particle(100, 300, 1.0);

    a->radius = 6;
    b->radius = 6;
    c->radius = 6;
    d->radius = 6;

    particles.push_back(a);
    particles.push_back(b);
    particles.push_back(c);
    particles.push_back(d);
    
    // Particle* smallBall = new Particle(50, 100, 1.0);
    // smallBall->radius = 4;
    // particles.push_back(smallBall);

    // Particle* bigBall = new Particle(200, 200, 3.0);
    // bigBall->radius = 12;
    // particles.push_back(bigBall);

    // Particle* smallPlanet = new Particle(200, 200, 1.0);
    // smallPlanet->radius = 6;
    // particles.push_back(smallPlanet);

    // Particle* bigPlanet = new Particle(500, 500, 20.0);
    // bigPlanet->radius = 20;
    // particles.push_back(bigPlanet);


    // for (int i = 0; i < 30; i++) {
    //     Particle* b = new Particle(200 + (i + 20), 200 + (i + 20), 1.0 + (0.1*i));
    //     b->radius = 4 + i;
    //     particles.push_back(b);
    // }

    // liquid.x = 0;
    // liquid.y = Graphics::Height() / 2;
    // liquid.w = Graphics::Width();
    // liquid.h = Graphics::Height() / 2;
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
                if (event.key.keysym.sym == SDLK_UP) pushForce.y = -50 * PIXELS_PER_METER;
                if (event.key.keysym.sym == SDLK_RIGHT) pushForce.x = 50 * PIXELS_PER_METER;
                if (event.key.keysym.sym == SDLK_DOWN) pushForce.y = 50 * PIXELS_PER_METER;
                if (event.key.keysym.sym == SDLK_LEFT) pushForce.x = -50 * PIXELS_PER_METER;
                break;
            case SDL_KEYUP:
                if (event.key.keysym.sym == SDLK_UP) pushForce.y = 0;
                if (event.key.keysym.sym == SDLK_RIGHT) pushForce.x = 0;
                if (event.key.keysym.sym == SDLK_DOWN) pushForce.y = 0;
                if (event.key.keysym.sym == SDLK_LEFT) pushForce.x = 0;
                break;
            // case SDL_MOUSEBUTTONDOWN:
            //     if (event.button.button == SDL_BUTTON_LEFT) {
            //         int x, y;
            //         SDL_GetMouseState(&x, &y);
            //         Particle* particle = new Particle(x, y, 1.0);
            //         particle->radius = 5;
            //         particles.push_back(particle);
            //     }
            //     break;
            case SDL_MOUSEMOTION:
                mouseCursor.x = event.motion.x;
                mouseCursor.y = event.motion.y;
            case SDL_MOUSEBUTTONDOWN:
                if (!leftMouseButtonDown && event.button.button == SDL_BUTTON_LEFT) {
                    leftMouseButtonDown = true;
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    mouseCursor.x = x;
                    mouseCursor.y = y;
                }
                break;
            case SDL_MOUSEBUTTONUP:
                if (leftMouseButtonDown && event.button.button == SDL_BUTTON_LEFT) {
                    leftMouseButtonDown = false;
                    int lastParticle = NUM_PARTICLES - 1;
                    Vec2 impulseDirection = (particles[lastParticle]->position - mouseCursor).UnitVector();
                    float impulseMagnitude = (particles[lastParticle]->position - mouseCursor).Magnitude() * 5.0;
                    particles[lastParticle]->velocity = impulseDirection * impulseMagnitude;

                    // Vec2 impulseDirection = (particles[0]->position - mouseCursor).UnitVector();
                    // float impulseMagnitude = (particles[0]->position - mouseCursor).Magnitude() * 5.0;
                    // particles[0]->velocity = impulseDirection * impulseMagnitude;
                }
                break;
        }
    }
}

void Application::Update() {
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
    
    // particle->velocity = Vec2(2.0, 0.0);
    // particle->position += particle->velocity;

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

    particles[0]->AddForce(pushForce);

    // particle->acceleration = Vec2(0.0, 9.8 * PIXELS_PER_METER);
    for (auto particle : particles) {
        // Vec2 wind = Vec2(0.2 * PIXELS_PER_METER, 0.0); // ветер
        // particle->AddForce(wind);
        // Vec2 weight = Vec2(0.0, particle->mass * 9.8 * PIXELS_PER_METER); // Сила тяжести
        // particle->AddForce(weight);
        // particle->AddForce(pushForce); // Сила из клавиатуры

        Vec2 drag = Force::GenerateDragForce(*particle, 0.003);
        particle->AddForce(drag);

        Vec2 weight = Vec2(0.0, particle->mass * 9.8 * PIXELS_PER_METER);
        particle->AddForce(weight);
        // Только если в воде
        // if (particle->position.y >= liquid.y) {
        //     Vec2 drag = Force::GenerateDragForce(*particle, 0.01);
        //     particle->AddForce(drag);
        // }
    }

    Vec2 ab = Force::GenerateSpringForce(*particles[0], *particles[1], restLength, k);
    particles[0]->AddForce(ab);
    particles[1]->AddForce(-ab);

    Vec2 bc = Force::GenerateSpringForce(*particles[1], *particles[2], restLength, k);
    particles[1]->AddForce(bc);
    particles[2]->AddForce(-bc);

    Vec2 cd = Force::GenerateSpringForce(*particles[2], *particles[3], restLength, k);
    particles[2]->AddForce(cd);
    particles[3]->AddForce(-cd);

    Vec2 da = Force::GenerateSpringForce(*particles[3], *particles[0], restLength, k);
    particles[3]->AddForce(da);
    particles[0]->AddForce(-da);

    Vec2 ac = Force::GenerateSpringForce(*particles[0], *particles[2], restLength, k);
    particles[0]->AddForce(ac);
    particles[2]->AddForce(-ac);

    Vec2 bd = Force::GenerateSpringForce(*particles[1], *particles[3], restLength, k);
    particles[1]->AddForce(bd);
    particles[3]->AddForce(-bd);

    // Vec2 springForce = Force::GenerateSpringForce(*particles[0], anchor, restLength, k);
    // particles[0]->AddForce(springForce);

    // for (int i = 1; i < NUM_PARTICLES; i++) {
    //     int currParticle = i;
    //     int prevParticle = i - 1;
    //     Vec2 springForce = Force::GenerateSpringForce(*particles[currParticle], *particles[prevParticle], restLength, k);
    //     particles[currParticle]->AddForce(springForce);
    //     particles[prevParticle]->AddForce(-springForce);
    // }

    // Vec2 attraction = Force::GenerateGravitationalForce(*particles[0], *particles[1], 1000.0, 5, 100); // сила притяжения
    // particles[0]->AddForce(attraction);
    // particles[1]->AddForce(-attraction);

    for (auto particle : particles) {
        particle->Integrate(deltaTime);
    }

    // Проверка границ окна
    for (auto particle : particles) {
        // Если о левый борт
        if (particle->position.x - particle->radius <= 0) {
            particle->position.x = particle->radius;
            particle->velocity.x *= -0.9; // меняем направление скорости с помощью знака -
        }
        // Если о правый борт 
        else if (particle->position.x + particle->radius >= Graphics::Width()) {
            particle->position.x = Graphics::Width() - particle->radius;
            particle->velocity.x *= -0.9; 
        }
        // Если о потолок
        if (particle->position.y - particle->radius <= 0) {
            particle->position.y = particle->radius;
            particle->velocity.y *= -0.9;
        }
        // Если о землю
        else if (particle->position.y + particle->radius >= Graphics::Height()) {
            particle->position.y = Graphics::Height() - particle->radius;
            particle->velocity.y *= -0.9; 
        }
    }
}

void Application::Render() {
    Graphics::ClearScreen(0xFF0F0721);

    if (leftMouseButtonDown) {
        int lastParticle = NUM_PARTICLES - 1;
        Graphics::DrawLine(particles[lastParticle]->position.x, particles[lastParticle]->position.y, mouseCursor.x, mouseCursor.y, 0xFF0000FF);
    }

    Graphics::DrawLine(particles[0]->position.x, particles[0]->position.y, particles[1]->position.x, particles[1]->position.y, 0xFF313131);
    Graphics::DrawLine(particles[1]->position.x, particles[1]->position.y, particles[2]->position.x, particles[2]->position.y, 0xFF313131);
    Graphics::DrawLine(particles[2]->position.x, particles[2]->position.y, particles[3]->position.x, particles[3]->position.y, 0xFF313131);
    Graphics::DrawLine(particles[3]->position.x, particles[3]->position.y, particles[0]->position.x, particles[0]->position.y, 0xFF313131);
    Graphics::DrawLine(particles[0]->position.x, particles[0]->position.y, particles[2]->position.x, particles[2]->position.y, 0xFF313131);
    Graphics::DrawLine(particles[1]->position.x, particles[1]->position.y, particles[3]->position.x, particles[3]->position.y, 0xFF313131);

    Graphics::DrawFillCircle(particles[0]->position.x, particles[0]->position.y, particles[0]->radius, 0xFFEEBB00);
    Graphics::DrawFillCircle(particles[1]->position.x, particles[1]->position.y, particles[1]->radius, 0xFFEEBB00);
    Graphics::DrawFillCircle(particles[2]->position.x, particles[2]->position.y, particles[2]->radius, 0xFFEEBB00);
    Graphics::DrawFillCircle(particles[3]->position.x, particles[3]->position.y, particles[3]->radius, 0xFFEEBB00);


    // Draw the anchor and the spring to the first bob
    // Graphics::DrawFillCircle(anchor.x, anchor.y, 5, 0xFF001155);
    // Graphics::DrawLine(anchor.x, anchor.y, particles[0]->position.x, particles[0]->position.y, 0xFF313131);

    // // Draw all the springs from one particle to the next
    // for (int i = 0; i < NUM_PARTICLES - 1; i++) {
    //     int currParticle = i;
    //     int nextParticle = i + 1;
    //     Graphics::DrawLine(particles[currParticle]->position.x, particles[currParticle]->position.y, particles[nextParticle]->position.x, particles[nextParticle]->position.y, 0xFF313131);
    // }

    // // Draw all the bob particles
    // for (auto particle: particles) {
    //     Graphics::DrawFillCircle(particle->position.x, particle->position.y, particle->radius, 0xFFEEBB00);
    // }

    Graphics::RenderFrame();
}

void Application::Destroy() {
    for (auto particle : particles) {
        delete particle;
    }
    Graphics::CloseWindow();
}