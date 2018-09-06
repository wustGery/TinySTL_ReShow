#include "typetraits.h"
#include "iterator.h"

namespace TinySTL
{
	template<class InputIterator>
	typename iterator_traits<InputIterator>::difference_type
		_distance(InputIterator first, InputIterator last, input_iterator_tag){
			typename iterator_traits<InputIterator>::difference_type dist = 0;
			while (first++ != last){
				++dist;
			}
			return dist;
		}

	template<class RandomIterator>
	typename iterator_traits<RandomIterator>::difference_type
		_distance(RandomIterator first, RandomIterator last, random_access_iterator_tag){
			auto dist = last - first;
			return dist;

		}

	template<class Iterator>
	typename iterator_traits<Iterator>::difference_type
		distance(Iterator first, Iterator last){
			typedef typename iterator_traits<Iterator>::iterator_category iterator_category;
			return _distance(first, last, iterator_category());
		}
}