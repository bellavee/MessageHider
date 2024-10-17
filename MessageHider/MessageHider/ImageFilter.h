#pragma once

#include "Image.h"

class ImageFilter {
public:
    virtual ~ImageFilter() = default;
    virtual void Apply(Image& image) const = 0;
};

class BrightnessFilter : public ImageFilter {
public:
    explicit BrightnessFilter(int value) : m_value(value) {}
    void Apply(Image& image) const override;

private:
    int m_value;
};

class ContrastFilter : public ImageFilter {
public:
    explicit ContrastFilter(float factor) : m_factor(factor) {}
    void Apply(Image& image) const override;

private:
    float m_factor;
};

class SaturationFilter : public ImageFilter {
public:
    explicit SaturationFilter(float factor) : m_factor(factor) {}
    void Apply(Image& image) const override;

private:
    float m_factor;
};