#pragma once

#include "Filter.hpp"
#include "../Geometry/Rectangle.hpp"
#include "../RenderTarget.hpp"

/** @ingroup Core
 * @{
 */

/// Applies post-processing effects to the rendered image.
class PostProcessing {
    public:
        /// Create new post-processing handler.
        /**
         * @param size Size of the context.
         */
        PostProcessing(const glm::vec2& size);
        
        /// Free allocated resources.
        ~PostProcessing();
        
        /// Get render target.
        /**
         * @return Target to render to
         */
        RenderTarget* GetRenderTarget() const;
        
        /// Apply a filter to the render image.
        /**
         * filter %Filter to apply.
         */
        void ApplyFilter(Filter* filter);
        
        /// Render resulting image to screen.
        void Render();
        
    private:
        short int which;
        
        RenderTarget* buffers[2];
        
        Geometry::Rectangle* rectangle;
};

/** @} */
