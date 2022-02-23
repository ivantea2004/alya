#pragma once
#include<alya/graphics/core/attribute_stream.hpp>
#include<alya/graphics/core/vertex_stream_base.hpp>

namespace alya::graphics::core
{

	
	struct attribute_semantic
	{
		const char* name;
		size_t index;
	};

	template<typename...T>
	class vertex_stream : public vertex_stream_base
	{
	public:

		vertex_stream(const std::array<attribute_semantic, sizeof...(T)>&sematics, const vertex_shader&shader, context_base&context)
			: vertex_stream(std::make_index_sequence<sizeof...(T)>{}, sematics, shader, context)
		{}

		template<size_t I>
		void attach(attribute_stream<std::tuple_element_t<I, std::tuple<T...>>> a)
		{
			attach(std::move(a), I);
		}
		
		template<size_t I, typename U>
		void attach(attribute_stream<U> a) requires compatible_pixel<pixel_traits<U>, pixel_traits<std::tuple_element_t<I, std::tuple<T...>>>>
		{
			attach(std::move(a), I);
		}

	private:

		void attach(attribute_stream_base&& stream, size_t index)
		{
			attributes_.at(index) = std::move(stream);
		}

		template<size_t...I>
		vertex_stream(std::index_sequence<I...>, const std::array<attribute_semantic, sizeof...(I)>&semantic, const vertex_shader&shader, context_base&context)
			: vertex_stream_base({ details::attribute_signature{details::make_pixel_type<std::tuple_element_t<I, std::tuple<T...>>>(), semantic[I].name, semantic[I].index}... }, shader, context)
		{}

		std::array<attribute_stream_base, sizeof...(T)> attributes_;
		friend class context_base;
	};

}
