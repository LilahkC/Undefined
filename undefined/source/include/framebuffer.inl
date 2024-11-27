template <size_t TextureNumber>

Framebuffer* Framebuffer::Create(float width, float height)
{
    static_assert(TextureNumber < 32, "TextureNumber must be less than 32");

    unsigned int attachments[TextureNumber] = { 0 };

    Framebuffer* f = new Framebuffer();

    f->Width = width;
    f->Height = height;

    Renderer* mRenderer = ServiceLocator::Get<Renderer>();

    mRenderer->GenerateFramebuffer(1, &f->FBO_ID);
    mRenderer->BindFramebuffer(GL_FRAMEBUFFER, f->FBO_ID);

    // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
    mRenderer->GenerateRenderbuffer(1, &f->RBO_ID);
    mRenderer->BindRenderbuffer(f->RBO_ID);
    mRenderer->SetRenderBufferStorageData(GL_DEPTH24_STENCIL8, f->Width, f->Height); // use a single renderbuffer object for both a depth AND stencil buffer.

    mRenderer->BindRenderbufferToFramebuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, f->RBO_ID);

    f->FramebufferTextures.resize(TextureNumber);

    for (int i = 0; i < TextureNumber; i++)
    {
        if (i == 0)
        {
            f->FramebufferTextures[0] = std::make_unique<Texture>(f->Width, f->Height);
        }

        else
        {
            f->FramebufferTextures[i] = std::make_unique<Texture>(f->Width, f->Height, GL_R32I, GL_RED_INTEGER);
        }

        mRenderer->BindTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, f->FramebufferTextures[i]->GetID());
        attachments[i] = GL_COLOR_ATTACHMENT0 + i;
    }

    glDrawBuffers(TextureNumber, attachments);

    mRenderer->BindFramebuffer(GL_FRAMEBUFFER, f->FBO_ID);
    // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        Logger::Error("Framebuffer is not complete! : {}", glGetError());
    }

    mRenderer->BindFramebuffer(GL_FRAMEBUFFER, 0);

    return f;
}